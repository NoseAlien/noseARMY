#pragma once

#include "ADXComponent.h"
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <string>

//音声データをロードし再生するコンポーネント
class ADXAudioSource : public ADXComponent
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

public:
	float volume_ = 1.0f;
	bool useDistanceFade_ = false;
	float startFadeDistance_ = 0.5f;
	float endFadeDistance_ = 1.0f;
	float endFadeVolume_ = 0.0f;

private:
	std::string name_ = {};
	uint32_t sHandle_ = 0;
	SoundData data_{};
	XAUDIO2_BUFFER buf_{};
	IXAudio2SourceVoice* pSourceVoice_ = nullptr;

public:
	//音声データ読み込み
	void LoadADXAudio(const std::string& filename);

	//音を再生、ループ再生するなら引数のloopをtrueに
	void Play(bool loop = false);

	//再生中の音を停止、後で続きから再生するなら引数のpauseをtrueに
	void Stop(bool pause = false);

	//音を再生中ならtrueを返す
	bool IsPlaying();

	//音声データのハンドルを取得
	uint32_t GetSHandle() { return sHandle_; };

	//更新処理
	void UniqueUpdate();

private:
	static Microsoft::WRL::ComPtr<IXAudio2> S_xAudio2;
	static IXAudio2MasteringVoice* S_masterVoice;
	static uint32_t S_currentSHandle;

public:
	//クラスを初期化
	static void StaticInitialize();

	//クラスの終了処理
	static void StaticFinalize();
};