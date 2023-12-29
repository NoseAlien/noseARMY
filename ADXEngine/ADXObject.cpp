#include "ADXObject.h"
#include "ADXCommon.h"
#include "ADXSceneManager.h"
#include "ADXCamera.h"
#include "ADXRenderer.h"
#include <d3dcompiler.h>
#include <imgui.h>

#pragma comment(lib, "d3dcompiler.lib")

uint64_t ADXObject::S_descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ADXObject::S_cmdList = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> ADXObject::S_rootSignature;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ADXObject::S_pipelineState;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ADXObject::S_pipelineStateAlpha;

uint64_t ADXObject::S_GpuStartHandle = 0;
std::list<std::unique_ptr<ADXObject, ADXUtility::NPManager<ADXObject>>> ADXObject::S_objs{};
std::vector<ADXCamera*> ADXObject::S_allCameraPtr{};
ADXVector3 ADXObject::S_limitPos1 = { -300,-300,-100 };
ADXVector3 ADXObject::S_limitPos2 = { 100,100,150 };
bool ADXObject::S_highQualityZSort = false;


void ADXObject::StaticInitialize()
{
	// パイプライン初期化
	InitializeGraphicsPipeline();
}

void ADXObject::Initialize()
{
	*this = {};
	CreateConstBuffer();
	transform_.Initialize(this);
}

void ADXObject::CreateConstBuffer()
{
	InitializeConstBufferMaterial(constBuffB1_);
}

void ADXObject::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	//頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shader/OBJVertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);

	//上の読み込みでエラーが起きたら出力ウィンドウに内容を表示
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";

		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//ピクセルシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shader/OBJPixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	//上の読み込みでエラーが起きたら出力ウィンドウに内容を表示
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";

		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//三次元座標
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//法線ベクトル
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//uv座標
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};

	//グラフィックスパイプラインの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; //背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineDesc.RasterizerState.DepthClipEnable = true;

	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true; //深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; //書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //同じか小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineDesc.BlendState.AlphaToCoverageEnable = true;

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定（三角形）
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc.SampleDesc.Count = 1;

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;//一度の描画に使う画像が1枚なので
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメーターの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//種類
	rootParams[0].Descriptor.ShaderRegister = 0;//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える
	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//種類
	rootParams[2].Descriptor.ShaderRegister = 1;//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し(タイリング)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し(タイリング)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し(タイリング)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;//全てリニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダーからのみ使用可能

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;//ルートパラメーターの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);//ルートパラメーター数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();

	//ルートシグネチャのシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	//ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&S_rootSignature));
	assert(SUCCEEDED(result));
	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = S_rootSignature.Get();
	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&S_pipelineState));
	assert(SUCCEEDED(result));

	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //書き込み不可

	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&S_pipelineStateAlpha));
	assert(SUCCEEDED(result));
}

void ADXObject::InitializeConstBufferTransform(Microsoft::WRL::ComPtr<ID3D12Resource>& constBuffTransform, ConstBufferDataTransform** constMapTransform)
{
	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();

	if (device != nullptr)
	{
		HRESULT result = S_FALSE;

		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataB0) + 0xff) & ~0xff;//256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//定数バッファの生成
		result = device->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));

		//定数バッファのマッピング
		result = constBuffTransform->Map(0, nullptr, (void**)constMapTransform);//マッピング
		assert(SUCCEEDED(result));
	}
}

void ADXObject::InitializeConstBufferMaterial(Microsoft::WRL::ComPtr<ID3D12Resource>& constBuff)
{
	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();

	if (device != nullptr)
	{
		HRESULT result = S_FALSE;

		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataB1) + 0xff) & ~0xff;//256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//定数バッファの生成
		result = device->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff));
		assert(SUCCEEDED(result));
	}
}

void ADXObject::SetAllCameraPtr(ADXCamera* camPtr)
{
	S_allCameraPtr.push_back(camPtr);
}

ADXObject* ADXObject::Create(const ADXVector3& setLocalPosition, const ADXQuaternion& setLocalRotation,
	const ADXVector3& setLocalScale, ADXWorldTransform* setParent)
{
	//オブジェクトを生成
	std::unique_ptr<ADXObject, ADXUtility::NPManager<ADXObject>> obj(new ADXObject);
	obj->Initialize();
	//引数のトランスフォーム情報を代入
	obj->transform_.localPosition_ = setLocalPosition;
	obj->transform_.localRotation_ = setLocalRotation;
	obj->transform_.localScale_ = setLocalScale;
	obj->transform_.parent_ = setParent;
	//オブジェクトリストに追加
	S_objs.push_back(move(obj));
	//ここで生成したオブジェクトを返す
	return S_objs.back().get();
}

ADXObject* ADXObject::Duplicate(const ADXObject& prefab)
{
	ADXObject* ret = Create(prefab.transform_.localPosition_, prefab.transform_.localRotation_,
		prefab.transform_.localScale_, prefab.transform_.parent_);
	//オブジェクトの情報を複製
	ret->transform_.rectTransform_ = prefab.transform_.rectTransform_;
	ret->transform_.modelPosition_ = prefab.transform_.modelPosition_;
	ret->transform_.modelRotation_ = prefab.transform_.modelRotation_;
	ret->transform_.modelScale_ = prefab.transform_.modelScale_;
	ret->model_ = prefab.model_;
	ret->material_ = prefab.material_;
	ret->texture_ = prefab.texture_;
	ret->renderLayer_ = prefab.renderLayer_;
	ret->sortingOrder_ = prefab.sortingOrder_;
	ret->alphaTex_ = prefab.alphaTex_;
	ret->isVisible_ = prefab.isVisible_;
	ret->isActive_ = prefab.isActive_;
	ret->useDefaultDraw_ = prefab.useDefaultDraw_;
	//ここで生成したオブジェクトを返す
	return ret;
}

void ADXObject::Update()
{
	//アクティブなオブジェクトの全コンポーネントの更新処理を呼ぶ
	if (isActive_)
	{
		for (auto& itr : components_)
		{
			itr->Update(this);
		}
	}
}

void ADXObject::StaticUpdate()
{
	ADXVector3 limitMinPos = { min(S_limitPos1.x_,S_limitPos2.x_),min(S_limitPos1.y_,S_limitPos2.y_) ,min(S_limitPos1.z_,S_limitPos2.z_) };
	ADXVector3 limitMaxPos = { max(S_limitPos1.x_,S_limitPos2.x_),max(S_limitPos1.y_,S_limitPos2.y_) ,max(S_limitPos1.z_,S_limitPos2.z_) };

	//オブジェクトの全コンポーネントのメモリ管理用処理を呼ぶ
	for (auto& itr : S_objs)
	{
		for (auto& comItr : itr->components_)
		{
			comItr->SafetyPhase();
		}
	}

	//消す準備が出来ているオブジェクトを削除
	for (auto& itr : S_objs)
	{
		if (itr->deleteFlag_ || (itr->transform_.parent_ != nullptr && itr->transform_.parent_->GetGameObject()->deleteFlag_))
		{
			itr->components_.clear();
		}
	}
	S_objs.remove_if([=](auto& itr)
		{ return itr->deleteFlag_; });

	for (auto& itr : S_objs)
	{
		//全オブジェクトの情報をImGuiに表示
		#ifdef _DEBUG
		float pos[3] = { itr->transform_.localPosition_.x_,itr->transform_.localPosition_.y_,itr->transform_.localPosition_.z_ };

		bool tool_active = true;
		ImGui::Begin("My First Tool", &tool_active, ImGuiWindowFlags_MenuBar);
		ImGui::InputFloat3("Position", pos);

		ImGui::End();

		itr->transform_.localPosition_ = { pos[0],pos[1],pos[2] };
		#endif

		//更新処理
		itr->Update();
		//オブジェクトが存在できる限界範囲内に収める
		if (itr->transform_.parent_ == nullptr && !itr->transform_.rectTransform_)
		{
			ADXVector3 itrWorldPos = itr->transform_.GetWorldPosition();
			itr->transform_.SetWorldPosition({
				max(limitMinPos.x_, min(itrWorldPos.x_, limitMaxPos.x_)),
				max(limitMinPos.y_, min(itrWorldPos.y_, limitMaxPos.y_)),
				max(limitMinPos.z_, min(itrWorldPos.z_, limitMaxPos.z_))
				});
		}
	}
}

void ADXObject::StaticDraw()
{
	//描画前処理
	PreDraw();

	std::list<ADXObject*> allObjPtr = GetObjs();

	//カメラの描画前処理
	for (int32_t i = 0; i < S_allCameraPtr.size(); i++)
	{
		S_allCameraPtr[i]->PrepareToRandering();
	}

	//全オブジェクトの描画前処理
	for (auto& itr : allObjPtr)
	{
		for (auto& comItr : itr->components_)
		{
			comItr->OnPreRender();
		}
	}

	int32_t minLayer = 0;
	int32_t maxLayer = 0;
	int32_t minSortingOrder = 0;
	int32_t maxSortingOrder = 0;

	if (allObjPtr.size() > 0)
	{
		minLayer = allObjPtr.front()->renderLayer_;
		maxLayer = allObjPtr.front()->renderLayer_;
		minSortingOrder = allObjPtr.front()->sortingOrder_;
		maxSortingOrder = allObjPtr.front()->sortingOrder_;
	}

	//全オブジェクトのレイヤー、描画順の範囲を読み取る
	for (auto& itr : allObjPtr)
	{
		if (itr->renderLayer_ < minLayer)
		{
			minLayer = itr->renderLayer_;
		}
		if (itr->renderLayer_ > maxLayer)
		{
			maxLayer = itr->renderLayer_;
		}

		if (itr->sortingOrder_ < minSortingOrder)
		{
			minSortingOrder = itr->sortingOrder_;
		}
		if (itr->sortingOrder_ > maxSortingOrder)
		{
			maxSortingOrder = itr->sortingOrder_;
		}
	}

	for (int32_t nowLayer = minLayer; nowLayer <= maxLayer; nowLayer++)
	{
		std::vector<ADXObject*> thisLayerObjPtr;

		// パイプラインステートの設定コマンド
		S_cmdList->SetPipelineState(S_pipelineState.Get());
		//全ピクセルの深度バッファ値を最奥の1.0にする
		S_cmdList->ClearDepthStencilView(*ADXCommon::GetCurrentInstance()->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		for (auto& itr : allObjPtr)
		{
			if (itr->renderLayer_ == nowLayer)
			{
				thisLayerObjPtr.push_back(itr);
			}
		}

		//ためたオブジェクトを深度の深い順に取り出しながら描画
		for (int32_t i = 0; thisLayerObjPtr.size() > 0; i++)
		{
			int32_t lowestSortingOrder = minSortingOrder;
			float dist = 0;
			int32_t target = 0;
			for (int32_t j = 0; j < thisLayerObjPtr.size(); j++)
			{
				thisLayerObjPtr[j]->transform_.UpdateMatrix();
				float zDepth;

				if (S_highQualityZSort)
				{
					float nearestVertDepth = 999;

					if (thisLayerObjPtr[j]->model_ != nullptr)
					{
						for (int32_t k = 0; k < thisLayerObjPtr[j]->model_->vertices_.size(); k++)
						{
							float VertDepth;
							ADXVector3 vertLocalPos = { thisLayerObjPtr[j]->model_->vertices_[k].pos.x,thisLayerObjPtr[j]->model_->vertices_[k].pos.y,thisLayerObjPtr[j]->model_->vertices_[k].pos.z };
							VertDepth = ADXMatrix4::Transform(ADXMatrix4::Transform(vertLocalPos, thisLayerObjPtr[j]->transform_.GetMatWorld()), ADXWorldTransform::GetViewProjection()).Length();

							if (VertDepth <= nearestVertDepth)
							{
								nearestVertDepth = VertDepth;
							}
						}
					}

					zDepth = nearestVertDepth;
				}
				else
				{
					zDepth = ADXMatrix4::Transform(thisLayerObjPtr[j]->transform_.GetWorldPosition(), ADXWorldTransform::GetViewProjection()).Length();
				}

				if (thisLayerObjPtr[j]->sortingOrder_ < lowestSortingOrder || thisLayerObjPtr[j]->sortingOrder_ == lowestSortingOrder && zDepth >= dist)
				{
					dist = zDepth;
					target = j;
					lowestSortingOrder = thisLayerObjPtr[j]->sortingOrder_;
				}
			}
			if (thisLayerObjPtr[target]->isVisible_ && thisLayerObjPtr[target]->isActive_)
			{
				thisLayerObjPtr[target]->Draw();
			}
			thisLayerObjPtr.erase(thisLayerObjPtr.begin() + target);
		}
	}
	PostDraw();
}

void ADXObject::PreDraw()
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ADXObject::S_cmdList == nullptr);

	// コマンドリストをセット
	ADXObject::S_cmdList = ADXCommon::GetCurrentInstance()->GetCommandList();

	// パイプラインステートの設定
	ADXObject::S_cmdList->SetPipelineState(S_pipelineState.Get());
	// ルートシグネチャの設定
	ADXObject::S_cmdList->SetGraphicsRootSignature(S_rootSignature.Get());
	// プリミティブ形状を設定
	ADXObject::S_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト


	ID3D12DescriptorHeap* srvHeap = ADXImage::GetSrvHeap();

	//SRVヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap };
	ADXObject::S_cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//SRVヒープの先頭にあるSRVをルートパラメーター1番に設定
	ADXObject::S_cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	S_GpuStartHandle = srvGpuHandle.ptr;

	// ルートシグネチャの設定コマンド
	ADXObject::S_cmdList->SetGraphicsRootSignature(S_rootSignature.Get());
}

void ADXObject::PostDraw()
{
	// コマンドリストを解除
	ADXObject::S_cmdList = nullptr;
	//ソート用配列を空にする
	S_allCameraPtr.clear();
}

std::list<ADXObject*> ADXObject::GetObjs()
{
	std::list<ADXObject*> ret = {};

	//ポインタに変換
	for (auto& itr : S_objs)
	{
		ret.push_back(itr.get());
	}

	return ret;
}

void ADXObject::Draw()
{
	//コンポーネントの描画前処理
	for (auto& itr : components_)
	{
		itr->OnWillRenderObject();
	}

	// nullptrチェック
	[[maybe_unused]]ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();
	assert(device);
	assert(S_cmdList);

	if (useDefaultDraw_ && model_ != nullptr)
	{
		HRESULT result = S_FALSE;
		//定数バッファへデータ転送
		ConstBufferDataB1* constMap1 = nullptr;
		result = constBuffB1_->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material_.ambient_;
		constMap1->diffuse = material_.diffuse_;
		constMap1->specular = material_.specular_;
		constMap1->alpha = material_.alpha_;
		constBuffB1_->Unmap(0, nullptr);

		//定数バッファビュー(CBV)の設定コマンド
		S_cmdList->SetGraphicsRootConstantBufferView(2, constBuffB1_->GetGPUVirtualAddress());

		D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;
		S_gpuDescHandleSRV.ptr = S_GpuStartHandle + texture_;
		S_cmdList->SetGraphicsRootDescriptorTable(1, S_gpuDescHandleSRV);
		
		transform_.UpdateConstBuffer();

		// 描画コマンド
		model_->Draw(transform_.constBuffTransform_.Get());
	}

	//コンポーネントの描画処理
	for (auto& itr : GetComponents<ADXRenderer>())
	{
		itr->Rendering();
	}

	//コンポーネントの描画後処理
	for (auto& itr : components_)
	{
		itr->Rendered();
	}
}

void ADXObject::Destroy()
{
	//このオブジェクトが次の更新処理前に消されるように設定
	deleteFlag_ = true;

	//コンポーネントの削除準備処理
	for (auto& itr : components_)
	{
		itr->OnDestroy();
	}
	//子オブジェクトにも行う
	for (auto& itr : transform_.GetChilds())
	{
		itr->GetGameObject()->Destroy();
	}
}

void ADXObject::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	//コンポーネントの接触処理
	for (auto& itr : components_)
	{
		itr->OnCollisionHit(col, myCol);
	}
}