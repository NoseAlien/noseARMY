﻿#pragma once

#include "ADXComponent.h"
#include "ADXMaterial.h"

//描画を行うコンポーネントの基底クラス
class ADXRenderer : public ADXComponent
{
public:
	struct ConstBufferDataB1 {
		DirectX::XMFLOAT3 ambient;
		float pad1;
		DirectX::XMFLOAT3 diffuse;
		float pad2;
		DirectX::XMFLOAT3 specular;
		float alpha;
	};

public:
	ADXMaterial material_{};

public:
	//描画処理
	void Rendering();

protected:
	// シェーダーの読み込みとコンパイル
	static void LoadShader(ID3DBlob** shaderBlob, LPCWSTR filePath, LPCSTR pEntryPoint);

	// グラフィックスパイプラインの初期値を自動生成して取得
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC CreateDefaultPipelineDesc(ID3DBlob* vsBlob, ID3DBlob* psBlob, D3D12_INPUT_ELEMENT_DESC inputLayout[], uint32_t numElements);

	// パイプラインステートを生成
	static void CreateGraphicsPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineDesc, ID3D12PipelineState** pipelineState);

private:
	//子クラス毎の固有描画処理
	virtual void UniqueRendering([[maybe_unused]]ID3D12Device* device, [[maybe_unused]] ID3D12GraphicsCommandList* cmdList) {};
};