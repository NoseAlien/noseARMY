#pragma once

#include <xaudio2.h>
#include <fstream>
#include <wrl.h>

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
	uint32_t bufferSize; // �o�b�t�@�̃T�C�Y
};

class ADXAudio
{
private:
	static Microsoft::WRL::ComPtr<IXAudio2> S_xAudio2;
	static IXAudio2MasteringVoice* S_masterVoice;

private:
	SoundData data{};
	XAUDIO2_BUFFER buf{};
	float volume = 1.0f;
	IXAudio2SourceVoice* pSourceVoice = nullptr;

public:
	static void StaticInitialize();
	static void Finalize();
	static ADXAudio SoundLoadWave(const char* filename);

public:
	void SoundPlayWave(bool loop = false);
	void SoundStopWave(bool pause = false);
	bool IsPlaying();
	void SetVolume(float setVolume);
	float GetVolume();
};