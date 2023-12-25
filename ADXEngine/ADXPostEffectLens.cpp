#include "d3dx12.h"
#include <d3dcompiler.h>
#include "ADXPostEffectLens.h"
#include "ADXWindow.h"
#include "ADXDataPool.h"
#include "ADXCommon.h"

#pragma comment(lib, "d3dcompiler.lib")


ADXPostEffectLens::ADXPostEffectLens()
{

}

void ADXPostEffectLens::UniqueInitialize()
{
	GetGameObject()->transform_.rectTransform_ = true;
	//GetGameObject()->transform.localScale_ = { 0.5f,0.5f,0.5f };
	GetGameObject()->transform_.UpdateMatrix();
	rect_ = ADXModel::CreateRect();
	GetGameObject()->texture_ = ADXImage::CreateADXImage(ADXWindow::S_window_width,ADXWindow::S_window_height);
	GetGameObject()->renderLayer_ = 100;
	GetGameObject()->model_ = &rect_;


	HRESULT result;

	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();
	ID3D12Resource* texBuff = ADXDataPool::GetImgData(GetGameObject()->texture_)->GetTexBuff();

	//・ｽf・ｽX・ｽN・ｽ・ｽ・ｽv・ｽ^・ｽq・ｽ[・ｽv・ｽﾌ設抵ｿｽ
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = 1;
	//・ｽf・ｽX・ｽN・ｽ・ｽ・ｽv・ｽ^・ｽq・ｽ[・ｽv・ｽﾌ撰ｿｽ・ｽ・ｽ
	result = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
	assert(SUCCEEDED(result));

	//・ｽ・ｽ・ｽ・ｽ・ｽ_・ｽ[・ｽ^・ｽ[・ｽQ・ｽb・ｽg・ｽr・ｽ・ｽ・ｽ[・ｽﾌ設抵ｿｽ
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//・ｽ・ｽ・ｽ・ｽ・ｽ_・ｽ[・ｽ^・ｽ[・ｽQ・ｽb・ｽg・ｽr・ｽ・ｽ・ｽ[・ｽﾌ撰ｿｽ・ｽ・ｽ
	device->CreateRenderTargetView(texBuff, &rtvDesc, descHeapRTV_->GetCPUDescriptorHandleForHeapStart());

	//・ｽ[・ｽx・ｽo・ｽb・ｽt・ｽ@・ｽﾌ設抵ｿｽ
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			ADXWindow::S_window_width,
			ADXWindow::S_window_height,
			1, 0, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	CD3DX12_HEAP_PROPERTIES heapTypeDefault = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	//・ｽ[・ｽx・ｽo・ｽb・ｽt・ｽ@・ｽﾌ作成
	result = device->CreateCommittedResource(
		&heapTypeDefault,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&depthBuff_)
		);
	assert(SUCCEEDED(result));

	//DSV・ｽp・ｽf・ｽX・ｽN・ｽ・ｽ・ｽv・ｽ^・ｽq・ｽ[・ｽv・ｽﾝ抵ｿｽ
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descHeapDesc.NumDescriptors = 1;
	//DSV・ｽp・ｽf・ｽX・ｽN・ｽ・ｽ・ｽv・ｽ^・ｽq・ｽ[・ｽv・ｽ・ｬ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	//・ｽf・ｽX・ｽN・ｽ・ｽ・ｽv・ｽ^・ｽq・ｽ[・ｽv・ｽ・ｽDSV・ｽ・ｬ
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart());

	CreateGraphicsPipelineState();
}

void ADXPostEffectLens::CreateGraphicsPipelineState()
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // ・ｽ・ｽ・ｽ_・ｽV・ｽF・ｽ[・ｽ_・ｽI・ｽu・ｽW・ｽF・ｽN・ｽg
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;	// ・ｽs・ｽN・ｽZ・ｽ・ｽ・ｽV・ｽF・ｽ[・ｽ_・ｽI・ｽu・ｽW・ｽF・ｽN・ｽg
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // ・ｽG・ｽ・ｽ・ｽ[・ｽI・ｽu・ｽW・ｽF・ｽN・ｽg

	//・ｽ・ｽ・ｽ_・ｽV・ｽF・ｽ[・ｽ_・ｽ[・ｽﾌ読み搾ｿｽ・ｽﾝとコ・ｽ・ｽ・ｽp・ｽC・ｽ・ｽ
	result = D3DCompileFromFile(
		L"Resources/shader/PostEffectTestVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);

	//・ｽ・ｽﾌ読み搾ｿｽ・ｽﾝでエ・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽﾍウ・ｽB・ｽ・ｽ・ｽh・ｽE・ｽﾉ難ｿｽe・ｽ・ｽ\・ｽ・ｽ
	if (FAILED(result))
	{
		//errorBlob・ｽ・ｽ・ｽ・ｽG・ｽ・ｽ・ｽ[・ｽ・ｽe・ｽ・ｽstring・ｽ^・ｽﾉコ・ｽs・ｽ[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";

		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//・ｽs・ｽN・ｽZ・ｽ・ｽ・ｽV・ｽF・ｽ[・ｽ_・ｽ[・ｽﾌ読み搾ｿｽ・ｽﾝとコ・ｽ・ｽ・ｽp・ｽC・ｽ・ｽ
	result = D3DCompileFromFile(
		L"Resources/shader/PostEffectTestPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	//・ｽ・ｽﾌ読み搾ｿｽ・ｽﾝでエ・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽﾍウ・ｽB・ｽ・ｽ・ｽh・ｽE・ｽﾉ難ｿｽe・ｽ・ｽ\・ｽ・ｽ
	if (FAILED(result))
	{
		//errorBlob・ｽ・ｽ・ｽ・ｽG・ｽ・ｽ・ｽ[・ｽ・ｽe・ｽ・ｽstring・ｽ^・ｽﾉコ・ｽs・ｽ[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";

		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//・ｽ・ｽ・ｽ_・ｽ・ｽ・ｽC・ｽA・ｽE・ｽg
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//・ｽO・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽW
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//・ｽ@・ｽ・ｽ・ｽx・ｽN・ｽg・ｽ・ｽ
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//uv・ｽ・ｽ・ｽW
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};

	//・ｽO・ｽ・ｽ・ｽt・ｽB・ｽb・ｽN・ｽX・ｽp・ｽC・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽﾌ設抵ｿｽ
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//・ｽV・ｽF・ｽ[・ｽ_・ｽ[・ｽﾌ設抵ｿｽ
	pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	//・ｽT・ｽ・ｽ・ｽv・ｽ・ｽ・ｽ}・ｽX・ｽN・ｽﾌ設抵ｿｽ
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//・ｽ・ｽ・ｽX・ｽ^・ｽ・ｽ・ｽC・ｽU・ｽﾌ設抵ｿｽ
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽO・ｽ・ｽ・ｽ・ｽ
	//・ｽf・ｽv・ｽX・ｽX・ｽe・ｽ・ｽ・ｽV・ｽ・ｽ・ｽX・ｽe・ｽ[・ｽg・ｽﾌ設抵ｿｽ
	pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; //・ｽ・ｽﾉ搾ｿｽ・ｽi

	//・ｽ・ｽ・ｽ・ｽ・ｽ_・ｽ[・ｽ^・ｽ[・ｽQ・ｽb・ｽg・ｽﾌブ・ｽ・ｽ・ｽ・ｽ・ｽh・ｽﾝ抵ｿｽ
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc;
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.LogicOpEnable = false;

	pipelineDesc.BlendState.RenderTarget[0] = blenddesc;

	//・ｽ[・ｽx・ｽo・ｽb・ｽt・ｽ@・ｽﾌフ・ｽH・ｽ[・ｽ}・ｽb・ｽg
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//・ｽ・ｽ・ｽ_・ｽ・ｽ・ｽC・ｽA・ｽE・ｽg・ｽﾌ設抵ｿｽ
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//・ｽ}・ｽ`・ｽﾌ形・ｽ・ｽﾝ抵ｿｽi・ｽO・ｽp・ｽ`・ｽj
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//・ｽ・ｽ・ｽﾌ托ｿｽ・ｽﾌ設抵ｿｽ
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc.SampleDesc.Count = 1;

	//・ｽf・ｽX・ｽN・ｽ・ｽ・ｽv・ｽ^・ｽ・ｽ・ｽ・ｽ・ｽW・ｽﾌ設抵ｿｽ
	CD3DX12_DESCRIPTOR_RANGE descriptorRange;
	descriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//t0・ｽ・ｽ・ｽW・ｽX・ｽ^

	//・ｽ・ｽ・ｽ[・ｽg・ｽp・ｽ・ｽ・ｽ・ｽ・ｽ[・ｽ^・ｽ[・ｽﾌ設抵ｿｽ
	CD3DX12_ROOT_PARAMETER rootParams[3] = {};
	//・ｽ關費ｿｽo・ｽb・ｽt・ｽ@0・ｽ・ｽ
	rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[1].InitAsDescriptorTable(1, &descriptorRange, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[2].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);

	
	//・ｽX・ｽ^・ｽe・ｽB・ｽb・ｽN・ｽT・ｽ・ｽ・ｽv・ｽ・ｽ・ｽ[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	//・ｽ・ｽ・ｽ[・ｽg・ｽV・ｽO・ｽl・ｽ`・ｽ・ｽ・ｽﾌ設抵ｿｽ
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	//・ｽo・ｽ[・ｽW・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾌシ・ｽ・ｽ・ｽA・ｽ・ｽ・ｽC・ｽY
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();

	//・ｽ・ｽ・ｽ[・ｽg・ｽV・ｽO・ｽl・ｽ`・ｽ・ｽ・ｽﾌ撰ｿｽ・ｽ・ｽ
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	//・ｽp・ｽC・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽﾉ・ｿｽ・ｽ[・ｽg・ｽV・ｽO・ｽl・ｽ`・ｽ・ｽ・ｽ・ｽZ・ｽb・ｽg
	pipelineDesc.pRootSignature = rootSignature_.Get();

	// ・ｽp・ｽC・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽX・ｽe・ｽ[・ｽg・ｽﾌ撰ｿｽ・ｽ・ｽ
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));
}

void ADXPostEffectLens::OnPreRender()
{
	ID3D12GraphicsCommandList* cmdList = ADXCommon::GetCurrentInstance()->GetCommandList();

	//・ｽ・ｽ・ｽ\・ｽ[・ｽX・ｽo・ｽ・ｽ・ｽA・ｽﾅ擾ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽﾝ可能・ｽﾉ変更
	CD3DX12_RESOURCE_BARRIER barrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(
		ADXDataPool::GetImgData(GetGameObject()->texture_)->GetTexBuff(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdList->ResourceBarrier(1, &barrierDesc);

	//・ｽ`・ｽ・ｽ・ｽﾌ変更
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = descHeapRTV_->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// ・ｽr・ｽ・ｽ・ｽ[・ｽ|・ｽ[・ｽg・ｽﾝ抵ｿｽ
	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f,
		(float)ADXWindow::S_window_width, (float)ADXWindow::S_window_height);
	cmdList->RSSetViewports(1, &viewPort);

	// ・ｽV・ｽU・ｽ[・ｽ・ｽ`・ｽﾝ抵ｿｽ
	CD3DX12_RECT scissorRect = CD3DX12_RECT(0, 0,
		ADXWindow::S_window_width, ADXWindow::S_window_height);
	cmdList->RSSetScissorRects(1, &scissorRect);

	//・ｽF・ｽﾆ深・ｽx・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	cmdList->ClearRenderTargetView(rtvHandle, ADXCommon::S_clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void ADXPostEffectLens::OnWillRenderObject()
{
	//・ｽ・ｽ・ｽ\・ｽ[・ｽX・ｽo・ｽ・ｽ・ｽA・ｽ・ｽﾟゑｿｽ
	CD3DX12_RESOURCE_BARRIER barrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(
		ADXDataPool::GetImgData(GetGameObject()->texture_)->GetTexBuff(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	ADXCommon::GetCurrentInstance()->GetCommandList()->ResourceBarrier(1, &barrierDesc);

	//・ｽ`・ｽ・ｽ・ｽ・ｽﾟゑｿｽ
	ADXCommon::GetCurrentInstance()->ReturnRenderTarget();
}