#pragma once

#include <xaudio2.h>
#include <fstream>
#include <wrl.h>

struct ChunkHeader // チャンクヘッダ
{
	char id[4]; // チャンク毎のID
	int32_t size; // チャンクサイズ
};

struct RiffHeader // RIFFヘッダチャンク
{
	ChunkHeader chunk; // "RIFF"
	char type[4]; // "WAVE"
};

struct FormatChunk // FMTチャンク
{
	ChunkHeader chunk; // "fmt "
	WAVEFORMATEX fmt; // 波形フォーマット
};

struct SoundData // 音声データ
{
	WAVEFORMATEX wfex; // 波形フォーマット
	std::unique_ptr<BYTE> pBuffer; // バッファの先頭アドレス
	uint32_t bufferSize; // バッファのサイズ
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