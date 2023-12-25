#pragma once

#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <string>

class ADXAudio
{
private:
	struct ChunkHeader // �`�����N�w�b�_
	{
		char id[4] = "";// �`�����N����ID
		int32_t size = 0; // �`�����N�T�C�Y
	};

	struct RiffHeader // RIFF�w�b�_�`�����N
	{
		ChunkHeader chunk{}; // "RIFF"
		char type[4] = ""; // "WAVE"
	};

	struct FormatChunk // FMT�`�����N
	{
		ChunkHeader chunk{}; // "fmt "
		WAVEFORMATEX fmt{}; // �g�`�t�H�[�}�b�g
	};

	struct SoundData // �����f�[�^
	{
		WAVEFORMATEX wfex{}; // �g�`�t�H�[�}�b�g
		std::unique_ptr<BYTE> pBuffer = nullptr; // �o�b�t�@�̐擪�A�h���X
		uint32_t bufferSize = 0; // �o�b�t�@�̃T�C�Y
	};

	std::string name_ = {};
	uint32_t sHandle_ = 0;
	SoundData data_{};
	XAUDIO2_BUFFER buf_{};
	float volume_ = 1.0f;
	IXAudio2SourceVoice* pSourceVoice_ = nullptr;

public:
	ADXAudio();
	void Play(bool loop = false);
	void Stop(bool pause = false);
	bool IsPlaying();
	void SetVolume(float setVolume);
	float GetVolume();
	uint32_t GetSHandle() { return sHandle_; };

private:
	static Microsoft::WRL::ComPtr<IXAudio2> S_xAudio2;
	static IXAudio2MasteringVoice* S_masterVoice;
	static uint32_t S_currentSHandle;

public:
	static void StaticInitialize();
	static void StaticFinalize();
	static ADXAudio LoadADXAudio(const std::string& filename);
};