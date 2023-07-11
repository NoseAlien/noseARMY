#pragma once

#include <xaudio2.h>
#include <fstream>
#include <wrl.h>

#pragma comment(lib,"xaudio2.lib")

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
	unsigned int bufferSize; // バッファのサイズ
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