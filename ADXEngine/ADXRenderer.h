#pragma once

#include "ADXComponent.h"
#include "ADXMaterial.h"

//描画を行うコンポーネントの基底クラス
class ADXRenderer : public ADXComponent
{
public:
	ADXMaterial material_{};

public:
	//描画処理
	void Rendering();

private:
	//子クラス毎の固有描画処理
	virtual void UniqueRendering([[maybe_unused]]ID3D12Device* device, [[maybe_unused]] ID3D12GraphicsCommandList* cmdList) {};
};