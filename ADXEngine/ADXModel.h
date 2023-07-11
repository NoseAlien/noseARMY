#pragma once

#include <vector>
#include <d3d12.h>
#include "ADXWorldTransform.h"
#include <wrl.h>
#include <string>

using namespace DirectX;

class ADXModel
{
private:
	static ID3D12Device* device;

public:
	static void StaticInitialize(ID3D12Device* setDevice);
	static ADXModel LoadModel(const std::string filePath);

public:
	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	std::vector<Vertex> vertices{};
	std::vector <uint16_t> indices{};

	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

	ADXModel();

	void SetNormal();

	void CreateVertexBufferView();
	void CreateIndexBufferView();

	void Initialize();

	void Draw(ID3D12GraphicsCommandList* commandList, ADXWorldTransform wtf_);
};