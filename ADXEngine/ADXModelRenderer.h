#pragma once

#include "ADXRenderer.h"
#include "ADXMaterial.h"
#include "ADXModel.h"

//モデルを描画するオブジェクトに入れるコンポーネント
class ADXModelRenderer : public ADXRenderer
{
public:
	ADXModel* model_ = nullptr;
	uint32_t texture_ = 0;

private:
	//描画処理
	void UniqueRendering(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

private:
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> S_pipelineState;

public:
	//クラスを初期化
	static void StaticInitialize();
};