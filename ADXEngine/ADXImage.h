#pragma once
#include <DirectXTex.h>
#include <d3d12.h>
#include <cassert>
#include <string>
#include <wrl.h>

using namespace DirectX;

class ADXImage
{
private:
	struct imgNameAndGH
	{
		std::string imgName;
		int GHandle;
	};

private:
	static D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	static UINT64 CpuStartHandle;
	static ID3D12Device* device;
	static UINT incrementSize;
	static std::vector<imgNameAndGH> imgDataPool;

public:
	static ADXImage LoadADXImage(std::string imgName);

public:
	ADXImage();
	int GetGHandle();

private:
	int Ghandle = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;

public:
	static void StaticInitialize(ID3D12Device* setDevice);
	static ID3D12DescriptorHeap* GetSrvHeap();
};