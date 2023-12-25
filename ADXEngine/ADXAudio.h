#pragma once

#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <string>

class ADXAudio
{
private:
	struct ChunkHeader // チャンクヘッダ
	{
		char id[4] = "";// チャンク毎のID
		int32_t size = 0; // チャンクサイズ
	};

	struct RiffHeader // RIFFヘッダチャンク
	{
		ChunkHeader chunk{}; // "RIFF"
		char type[4] = ""; // "WAVE"
	};

	struct FormatChunk // FMTチャンク
	{
		ChunkHeader chunk{}; // "fmt "
		WAVEFORMATEX fmt{}; // 波形フォーマット
	};

	struct SoundData // 音声データ
	{
		WAVEFORMATEX wfex{}; // 波形フォーマット
		std::unique_ptr<BYTE> pBuffer = nullptr; // バッファの先頭アドレス
		uint32_t bufferSize = 0; // バッファのサイズ
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