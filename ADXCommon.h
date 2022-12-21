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
	void Initialize(ADXWindow* setWindow);

	void PreDraw();
	void PostDraw();

	ID3D12Device* GetDevice() const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
	D3D12_CPU_DESCRIPTOR_HANDLE* GetDsvHandle() { return &dsvHandle; }


private:
	void InitializeFixFPS();
	void UpdateFixFPS();

	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapChain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();

private:
	ADXWindow* adxwindow = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;
	std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle{};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;
	std::chrono::steady_clock::time_point reference_;

};