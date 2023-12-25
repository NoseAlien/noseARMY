#include "ADXRenderer.h"
#include "ADXCommon.h"
#include "ADXObject.h"

void ADXRenderer::Rendering()
{
	// nullptrチェック
	[[maybe_unused]] ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();
	[[maybe_unused]] ID3D12GraphicsCommandList* cmdList = ADXObject::GetCmdList();
	assert(device);
	assert(cmdList);

	HRESULT result = S_FALSE;
	//定数バッファへデータ転送
	ADXObject::ConstBufferDataB1* constMap1 = nullptr;
	result = GetGameObject()->GetConstBuffB1()->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material_.ambient_;
	constMap1->diffuse = material_.diffuse_;
	constMap1->specular = material_.specular_;
	constMap1->alpha = material_.alpha_;
	GetGameObject()->GetConstBuffB1()->Unmap(0, nullptr);

	//定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(2, GetGameObject()->GetConstBuffB1()->GetGPUVirtualAddress());

	UniqueRendering(device, cmdList);
}
