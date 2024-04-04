#pragma once
#include <d3d12.h>
#include <string>
#include <wrl.h>

//画像データを格納するクラス
class ADXImage
{
private:
	std::string name_ = {};
	uint32_t Ghandle_ = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_ = nullptr;

public:
	//コンストラクタ
	ADXImage();

	//グラフィックハンドルを取得
	uint32_t GetGHandle() { return Ghandle_; };

	//テクスチャバッファを取得
	ID3D12Resource* GetTexBuff() { return texBuff_.Get(); };

	//名前を取得
	std::string GetName() { return name_; };

private:
	static D3D12_CPU_DESCRIPTOR_HANDLE S_srvHandle;
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> S_srvHeap;
	static uint64_t S_CpuStartHandle;
	static uint64_t S_incrementSize;

public:
	//クラスを初期化
	static void StaticInitialize();

	//SRVヒープを取得
	static ID3D12DescriptorHeap* GetSrvHeap() { return S_srvHeap.Get(); };

	//画像データ読み込み
	static uint32_t LoadADXImage(const std::string& imgName, bool generateMipMaps = false);

	//画像データを新規作成
	static uint32_t CreateADXImage(const uint64_t& width, const uint64_t& height, const std::string& imgName = "");
};