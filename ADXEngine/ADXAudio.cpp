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

void ADXAudio::StaticFinalize()
{
	S_xAudio2.Reset();
}

ADXAudio ADXAudio::LoadADXAudio(const std::string& filename)
{
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open("Resources/" + filename, std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o����
	assert(file.is_open());

	//RIFF�w�b�_�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	//Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Wave�t�@�C�������
	file.close();

	//return���邽�߂̉����f�[�^
	ADXAudio audioData = {};

	audioData.data_.wfex = format.fmt;
	audioData.data_.pBuffer.reset(reinterpret_cast<BYTE*>(pBuffer));
	audioData.data_.bufferSize = data.size;
	audioData.sHandle_ = S_currentSHandle;

	S_currentSHandle++;

	return audioData;
}

void ADXAudio::Play(bool loop)
{
	Stop();

	HRESULT result;

	//�Đ�����g�`�f�[�^�̐ݒ�
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
		//�g�`�t�H�[�}�b�g������SourceVoice�̐���
		pSourceVoice_ = nullptr;
		result = S_xAudio2->CreateSourceVoice(&pSourceVoice_, &data_.wfex);
		assert(SUCCEEDED(result));
	}

	pSourceVoice_->SetVolume(volume_);

	//�g�`�f�[�^�̍Đ�
	result = pSourceVoice_->SubmitSourceBuffer(&buf_);
	result = pSourceVoice_->Start();
}

void ADXAudio::Stop(bool pause)
{
	if (pSourceVoice_ != nullptr)
	{
		HRESULT result;

		//�g�`�f�[�^�̍Đ����~�߂�
		result = pSourceVoice_->Stop();
		if (!pause)
		{
			pSourceVoice_->DestroyVoice();
			pSourceVoice_ = nullptr;
		}
	}
}

bool ADXAudio::IsPlaying()
{
	if (pSourceVoice_ == nullptr)
	{
		return false;
	}

	XAUDIO2_VOICE_STATE xa2state;
	pSourceVoice_->GetState(&xa2state);

	return xa2state.BuffersQueued != 0;
}

void ADXAudio::SetVolume(float setVolume)
{
	volume_ = setVolume;
	if (pSourceVoice_ != nullptr)
	{
		pSourceVoice_->SetVolume(volume_);
	}
}

float ADXAudio::GetVolume()
{
	return volume_;
}