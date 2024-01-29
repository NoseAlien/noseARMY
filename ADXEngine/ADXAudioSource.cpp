#include "ADXAudioSource.h"
#include "ADXAudioListener.h"
#include "ADXObject.h"
#include "ADXUtility.h"
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

Microsoft::WRL::ComPtr<IXAudio2> ADXAudioSource::S_xAudio2 = nullptr;
IXAudio2MasteringVoice* ADXAudioSource::S_masterVoice = nullptr;
uint32_t ADXAudioSource::S_currentSHandle = 0;

void ADXAudioSource::StaticInitialize()
{
	HRESULT result;
	result = XAudio2Create(&S_xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = S_xAudio2->CreateMasteringVoice(&S_masterVoice);
}

void ADXAudioSource::StaticFinalize()
{
	S_xAudio2.Reset();
}

void ADXAudioSource::LoadADXAudio(const std::string& filename)
{
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open("Resources/" + filename, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());

	//RIFFヘッダの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	//Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Waveファイルを閉じる
	file.close();

	data_.wfex = format.fmt;
	data_.pBuffer.reset(reinterpret_cast<BYTE*>(pBuffer));
	data_.bufferSize = data.size;
	sHandle_ = S_currentSHandle;

	S_currentSHandle++;
}

void ADXAudioSource::Play(bool loop)
{
	Stop();

	HRESULT result;

	//再生する波形データの設定
	buf_ = {};
	buf_.pAudioData = data_.pBuffer.get();
	buf_.AudioBytes = data_.bufferSize;
	buf_.Flags = XAUDIO2_END_OF_STREAM;
	if (loop)
	{
		buf_.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else
	{
		buf_.LoopCount = 0;
	}

	if (pSourceVoice_ == nullptr)
	{
		//波形フォーマットを元にSourceVoiceの生成
		pSourceVoice_ = nullptr;
		result = S_xAudio2->CreateSourceVoice(&pSourceVoice_, &data_.wfex);
		assert(SUCCEEDED(result));
	}

	pSourceVoice_->SetVolume(volume_);

	//波形データの再生
	result = pSourceVoice_->SubmitSourceBuffer(&buf_);
	result = pSourceVoice_->Start();
}

void ADXAudioSource::Stop(bool pause)
{
	if (pSourceVoice_ != nullptr)
	{
		HRESULT result;

		//波形データの再生を止める
		result = pSourceVoice_->Stop();
		if (!pause)
		{
			pSourceVoice_->DestroyVoice();
			pSourceVoice_ = nullptr;
		}
	}
}

bool ADXAudioSource::IsPlaying()
{
	if (pSourceVoice_ == nullptr)
	{
		return false;
	}

	XAUDIO2_VOICE_STATE xa2state;
	pSourceVoice_->GetState(&xa2state);

	return xa2state.BuffersQueued != 0;
}

void ADXAudioSource::UniqueUpdate()
{
	float fadedVolume = volume_;
	ADXAudioListener* listener = ADXAudioListener::GetCurrentInstance();
	if (useDistanceFade_ && listener != nullptr)
	{
		float distanceFromListener = min(max(endFadeDistance_ * listener->radius_,
			(listener->GetGameObject()->transform_.GetWorldPosition() - GetGameObject()->transform_.GetWorldPosition()).Length())
			, startFadeDistance_ * listener->radius_);
		fadedVolume *= ADXUtility::ValueMapping(distanceFromListener,
			endFadeDistance_ * listener->radius_, startFadeDistance_ * listener->radius_, endFadeVolume_, 1);
	}
	if (pSourceVoice_ != nullptr)
	{
		pSourceVoice_->SetVolume(fadedVolume);
	}
}
