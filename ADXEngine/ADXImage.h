#pragma once
#include <d3d12.h>
#include <string>
#include <wrl.h>

class ADXImage
{

private:
	static D3D12_CPU_DESCRIPTOR_HANDLE S_srvHandle;
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> S_srvHeap;
	static uint64_t S_CpuStartHandle;
	static uint64_t S_incrementSize;

public:
	static uint32_t LoadADXImage(const std::string& imgName, bool generateMipMaps = false);
	static uint32_t CreateADXImage(const uint64_t& width, const uint64_t& height, const std::string& imgName = "");

public:
	ADXImage();
	uint32_t GetGHandle() { return Ghandle; };
	ID3D12Resource* GetTexBuff() { return texBuff.Get(); };

private:
	std::string name = {};
	uint32_t Ghandle = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;

public:
	static void StaticInitialize();
	static ID3D12DescriptorHeap* GetSrvHeap() { return S_srvHeap.Get(); };
};