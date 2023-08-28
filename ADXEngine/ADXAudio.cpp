#include "ADXAudio.h"
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

Microsoft::WRL::ComPtr<IXAudio2> ADXAudio::S_xAudio2 = nullptr;
IXAudio2MasteringVoice* ADXAudio::S_masterVoice = nullptr;
uint32_t ADXAudio::S_currentSHandle = 0;

ADXAudio::ADXAudio()
{
}

void ADXAudio::StaticInitialize()
{
	HRESULT result;
	result = XAudio2Create(&S_xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = S_xAudio2->CreateMasteringVoice(&S_masterVoice);
}

void ADXAudio::Finalize()
{
	S_xAudio2.Reset();
}

ADXAudio ADXAudio::LoadADXAudio(std::string filename)
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

	//returnするための音声データ
	ADXAudio audioData = {};

	audioData.data.wfex = format.fmt;
	audioData.data.pBuffer.reset(reinterpret_cast<BYTE*>(pBuffer));
	audioData.data.bufferSize = data.size;
	audioData.SHandle = S_currentSHandle;

	S_currentSHandle++;

	return audioData;
}

void ADXAudio::Play(bool loop)
{
	HRESULT result;

	//再生する波形データの設定
	buf = {};
	buf.pAudioData = data.pBuffer.get();
	buf.AudioBytes = data.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (loop)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else
	{
		buf.LoopCount = 0;
	}

	if (pSourceVoice == nullptr)
	{
		//波形フォーマットを元にSourceVoiceの生成
		pSourceVoice = nullptr;
		result = S_xAudio2->CreateSourceVoice(&pSourceVoice, &data.wfex);
		assert(SUCCEEDED(result));
	}

	pSourceVoice->SetVolume(volume);

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void ADXAudio::Stop(bool pause)
{
	if (pSourceVoice != nullptr)
	{
		HRESULT result;

		//波形データの再生を止める
		result = pSourceVoice->Stop();
		if (!pause)
		{
			pSourceVoice->DestroyVoice();
			pSourceVoice = nullptr;
		}
	}
}

bool ADXAudio::IsPlaying()
{
	if (pSourceVoice == nullptr)
	{
		return false;
	}

	XAUDIO2_VOICE_STATE xa2state;
	pSourceVoice->GetState(&xa2state);

	return xa2state.BuffersQueued != 0;
}

void ADXAudio::SetVolume(float setVolume)
{
	volume = setVolume;
	if (pSourceVoice != nullptr)
	{
		pSourceVoice->SetVolume(volume);
	}
}

float ADXAudio::GetVolume()
{
	return volume;
}