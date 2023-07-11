#pragma once

#include <xaudio2.h>
#include <fstream>
#include <wrl.h>

#pragma comment(lib,"xaudio2.lib")

struct ChunkHeader // �`�����N�w�b�_
{
	char id[4]; // �`�����N����ID
	int32_t size; // �`�����N�T�C�Y
};

struct RiffHeader // RIFF�w�b�_�`�����N
{
	ChunkHeader chunk; // "RIFF"
	char type[4]; // "WAVE"
};

struct FormatChunk // FMT�`�����N
{
	ChunkHeader chunk; // "fmt "
	WAVEFORMATEX fmt; // �g�`�t�H�[�}�b�g
};

struct SoundData // �����f�[�^
{
	WAVEFORMATEX wfex; // �g�`�t�H�[�}�b�g
	std::unique_ptr<BYTE> pBuffer; // �o�b�t�@�̐擪�A�h���X
	unsigned int bufferSize; // �o�b�t�@�̃T�C�Y
};

class ADXAudio
{
public:
	static Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice* masterVoice;

public:
	SoundData data{};

public:
	static void StaticInitialize();
	static ADXAudio SoundLoadWave(const char* filename);

public:
	void SoundPlayWave();
};