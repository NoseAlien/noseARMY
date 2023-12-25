#pragma once

#include <vector>
#include <d3d12.h>
#include "ADXWorldTransform.h"
#include <wrl.h>
#include <string>

class ADXModel
{
public:
	//頂点データ構造体
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

public:
	std::string name_ = {};

	std::vector<Vertex> vertices_{};
	std::vector <uint16_t> indices_{};

	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;

public:
	ADXModel();

	void SetNormal();

	void CreateVertexBufferView();
	void CreateIndexBufferView();

	void Initialize();

	void Draw(ID3D12Resource* constBuffTransform);

private:
	void Update();

public:
	static ADXModel LoadADXModel(const std::string& filePath);
	static ADXModel CreateRect();
};