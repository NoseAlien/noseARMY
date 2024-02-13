#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <chrono>
#include <thread>

#include "ADXWindow.h"

//DirectXを扱うクラス
class ADXCommon
{
public:
	static const float S_clearColor[4];
	static const uint32_t S_frameRate = uint64_t(1000000.0f / 60.0f);

private:
	ADXWindow* adxwindow = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;
	std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_{};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceVal_ = 0;
	std::chrono::steady_clock::time_point reference_;
	uint64_t elapsedMicroSeconds = 0;

public:
	//初期化処理
	void Initialize(ADXWindow* setWindow);

	//描画前処理
	void PreDraw();

	//描画後処理
	void PostDraw();

	//描画先の変更
	void ReturnRenderTarget();

	//デバイスを取得
	ID3D12Device* GetDevice() const { return device_.Get(); }

	//コマンドリストを取得
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }

	//DSVハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE* GetDsvHandle() { return &dsvHandle_; }

	//バックバッファの数を取得
	size_t GetBackBufferCount() const { return backBuffers_.size(); }

private:
	//フレームレート固定機能の初期化処理
	void InitializeFixFPS();

	//フレームレート固定機能の更新処理
	void UpdateFixFPS();

	//デバイスの初期化
	void InitializeDevice();

	//コマンドアロケーター、コマンドリスト、コマンドキューの初期化
	void InitializeCommand();

	//スワップチェーンの初期化
	void InitializeSwapChain();

	//レンダーターゲットビューの初期化
	void InitializeRenderTargetView();

	//深度バッファの初期化
	void InitializeDepthBuffer();

	//フェンスの初期化
	void InitializeFence();

	//コンストラクタ、デストラクタをprivateにしてシングルトンに

	ADXCommon() {};
	~ADXCommon() {};

	ADXCommon(const ADXCommon&) = delete;
	ADXCommon& operator=(const ADXCommon&) = delete;

private:
	static ADXCommon S_instance;

public:
	//インスタンスを取得
	static ADXCommon* GetInstance() { return &S_instance; }
};