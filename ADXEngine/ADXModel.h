#pragma once

#include <vector>
#include <d3d12.h>
#include "ADXWorldTransform.h"
#include <wrl.h>
#include <string>

//モデルデータを格納するクラス
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
	std::vector<Vertex> vertices_{};
	std::vector <uint16_t> indices_{};

private:
	std::string name_ = {};

	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;

public:
	//コンストラクタ
	ADXModel();

	std::string GetName() { return name_; }

	//法線を自動設定
	void SetNormal();

	//頂点バッファビュー作成
	void CreateVertexBufferView();

	//インデックスバッファビュー作成
	void CreateIndexBufferView();

	//初期化処理
	void Initialize();

	//モデルを描画
	void Draw(ID3D12Resource* constBuffTransform);

private:
	//更新処理
	void Update();

public:
	//モデルデータ読み込み
	static ADXModel LoadADXModel(const std::string& filePath);

	//画像表示用の正方形ポリゴンを作成
	static ADXModel CreateRect();
};