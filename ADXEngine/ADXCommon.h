#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <chrono>
#include <thread>

#include "ADXWindow.h"

class ADXCommon
{
public:
	static const float S_clearColor[4];

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

private:
	static ADXCommon* S_currentInstance;

public:
	void Initialize(ADXWindow* setWindow);

	void PreDraw();
	void PostDraw();

	void ReturnRenderTarget();

	ID3D12Device* GetDevice() const { return device_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }
	D3D12_CPU_DESCRIPTOR_HANDLE* GetDsvHandle() { return &dsvHandle_; }
	size_t GetBackBufferCount() const { return backBuffers_.size(); }

private:
	void InitializeFixFPS();
	void UpdateFixFPS();

	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapChain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();

public:
	static ADXCommon* GetCurrentInstance() { return S_currentInstance; };
};