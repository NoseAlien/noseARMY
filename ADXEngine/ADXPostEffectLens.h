#pragma once
#include "ADXObject.h"

//ポストエフェクトを描画するコンポーネント
class ADXPostEffectLens : public ADXComponent
{
private:
	ADXModel* rect_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV_ = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	// グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
public:
	//コンストラクタ
	ADXPostEffectLens();

	//初期化処理
	void UniqueInitialize();

	//パイプラインステートを作成
	void CreateGraphicsPipelineState();

private:
	//描画前処理
	void OnPreRender();

	//描画時処理
	void OnWillRenderObject();
};