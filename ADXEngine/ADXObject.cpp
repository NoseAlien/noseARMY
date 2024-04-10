#include "ADXObject.h"
#include "ADXCommon.h"
#include "ADXSceneManager.h"
#include "ADXCamera.h"
#include "ADXModelRenderer.h"
#include <d3dcompiler.h>
#include <imgui.h>

#pragma comment(lib, "d3dcompiler.lib")

const FLOAT ADXObject::clearDepth = 1.0f;
const UINT8 ADXObject::clearStencil = 0;

uint64_t ADXObject::S_descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ADXObject::S_cmdList = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> ADXObject::S_rootSignature;

uint64_t ADXObject::S_GpuStartHandle = 0;
std::list<std::unique_ptr<ADXObject>> ADXObject::S_objs{};
std::vector<ADXCamera*> ADXObject::S_allCameraPtr{};
ADXVector3 ADXObject::S_limitPos1 = { -300,-300,-100 };
ADXVector3 ADXObject::S_limitPos2 = { 300,300,300 };
ADXObject::drawQuality ADXObject::S_drawQuality = low;


void ADXObject::StaticInitialize()
{
	// パイプライン初期化
	InitializeGraphicsPipeline();
}

void ADXObject::Initialize()
{
	*this = {};
	transform_.Initialize(this);
}

void ADXObject::InitializeGraphicsPipeline()
{
	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;//一度の描画に使う画像が1枚なので
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

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

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;//ルートパラメーターの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);//ルートパラメーター数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	CreateRootSignature(&rootSignatureDesc);
}

void ADXObject::InitializeConstBufferTransform(ID3D12Resource** constBuffTransform, ConstBufferDataTransform** constMapTransform)
{
	ID3D12Device* device = ADXCommon::GetInstance()->GetDevice();

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
			IID_PPV_ARGS(constBuffTransform));
		assert(SUCCEEDED(result));

		//定数バッファのマッピング
		ID3D12Resource* constBuff = *constBuffTransform;

		result = constBuff->Map(0, nullptr, (void**)constMapTransform);//マッピング
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
	std::unique_ptr<ADXObject> obj(new ADXObject);
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
	ret->renderLayer_ = prefab.renderLayer_;
	ret->sortingOrder_ = prefab.sortingOrder_;
	ret->alphaTex_ = prefab.alphaTex_;
	ret->isVisible_ = prefab.isVisible_;
	ret->isActive_ = prefab.isActive_;
	//ここで生成したオブジェクトを返す
	return ret;
}

void ADXObject::Annihilate()
{
	for (auto& itr : S_objs)
	{
		itr->Destroy();
	}
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
		if (itr->GetDeleteFlag() || (itr->transform_.parent_ != nullptr && itr->transform_.parent_->GetGameObject()->GetDeleteFlag()))
		{
			itr->components_.clear();
		}
	}
	S_objs.remove_if([=](auto& itr)
		{ return itr->GetDeleteFlag(); });

	for (auto& itr : S_objs)
	{
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

		//全オブジェクトの情報をImGuiに表示
		float pos[3] = { itr->transform_.localPosition_.x_,itr->transform_.localPosition_.y_,itr->transform_.localPosition_.z_ };

		bool tool_active = true;
		ImGui::Begin("ADXObject", &tool_active, ImGuiWindowFlags_MenuBar);
		ImGui::InputFloat3("Position", pos);

		ImGui::End();

		itr->transform_.localPosition_ = { pos[0],pos[1],pos[2] };
	}
}

void ADXObject::StaticDraw()
{
	//描画前処理
	PreDraw();

	std::list<ADXObject*> allObjPtr = GetObjs();
	std::vector<renderLayerGroup> renderChart{};

	//カメラの描画前処理
	for (uint32_t i = 0; i < S_allCameraPtr.size(); i++)
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

	//同じrenderLayer_、sortingOrder_のオブジェクトのグループに分ける
	for (auto& itr : allObjPtr)
	{
		int64_t layerIndex = -1;
		int64_t sortIndex = -1;
		
		for (uint32_t i = 0; i < renderChart.size(); i++)
		{
			if (renderChart[i].renderLayer_ = itr->renderLayer_)
			{
				layerIndex = i;
				break;
			}
		}
		if (layerIndex < 0)
		{
			renderChart.push_back({ itr->renderLayer_,{} });
			layerIndex = renderChart.size() - 1;
		}

		std::vector<sortingOrderGroup>& sortChart = renderChart[layerIndex].groups_;
		for (uint32_t i = 0; i < sortChart.size(); i++)
		{
			if (sortChart[i].sortingOrder_ = itr->sortingOrder_)
			{
				sortIndex = i;
				break;
			}
		}
		if (sortIndex < 0)
		{
			sortChart.push_back({ itr->sortingOrder_,{} });
			sortIndex = sortChart.size() - 1;
		}

		renderChart[layerIndex].groups_[sortIndex].objs_.push_back(itr);
	}

	//renderLayer_、sortingOrder_の小さいグループから順に描画
	while (!renderChart.empty())
	{
		//全ピクセルの深度バッファ値とステンシル値を初期化
		S_cmdList->ClearDepthStencilView(*ADXCommon::GetInstance()->GetDsvHandle(),
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, clearDepth, clearStencil, 0, nullptr);

		//renderLayer_が最も小さいグループを探す
		uint32_t layerIndex = 0;
		int32_t minLayer = 0;
		for (int i = 0; i < renderChart.size(); i++)
		{
			if (i == 0 || (renderChart[i].renderLayer_ < minLayer))
			{
				layerIndex = i;
				minLayer = renderChart[i].renderLayer_;
			}
		}

		std::vector<sortingOrderGroup>& sortChart = renderChart[layerIndex].groups_;
		while (!sortChart.empty())
		{
			//sortingOrder_が最も小さいグループを探す
			uint32_t sortIndex = 0;
			int32_t minSort = 0;
			for (int i = 0; i < sortChart.size(); i++)
			{
				if (i == 0 || (sortChart[i].sortingOrder_ < minSort))
				{
					sortIndex = i;
					minSort = sortChart[i].sortingOrder_;
				}
			}

			std::vector<ADXObject*>& objs = sortChart[sortIndex].objs_;
			while (!objs.empty())
			{
				if (S_drawQuality != low)
				{
					//ためたオブジェクトを深度の深い順に取り出しながら描画
					uint32_t target = 0;
					float dist = 0;
					for (uint32_t i = 0; i < objs.size(); i++)
					{
						ADXObject* temp = objs[i];
						ADXModelRenderer* tempRenderer = temp->GetComponent<ADXModelRenderer>();

						float zDepth;

						if (S_drawQuality == high)
						{
							float nearestVertDepth = 999;

							if (tempRenderer->model_ != nullptr)
							{
								for (uint32_t j = 0; j < tempRenderer->model_->vertices_.size(); j++)
								{
									float VertDepth;
									ADXVector3 vertLocalPos = { tempRenderer->model_->vertices_[j].pos.x,
										tempRenderer->model_->vertices_[j].pos.y,tempRenderer->model_->vertices_[j].pos.z };
									VertDepth = ADXMatrix4::Transform(ADXMatrix4::Transform(vertLocalPos, temp->transform_.GetMatWorld()),
										ADXWorldTransform::GetViewProjection()).Length();

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
							zDepth = ADXMatrix4::Transform(temp->transform_.GetWorldPosition(), ADXWorldTransform::GetViewProjection()).Length();
						}

						if (zDepth >= dist)
						{
							dist = zDepth;
							target = i;
						}
					}

					objs[target]->Draw();
					objs.erase(objs.begin() + target);
				}
				else
				{
					//Zソート無しで描画
					for (auto& itr : objs)
					{
						itr->Draw();
					}
					objs.clear();
				}
			}
			sortChart.erase(sortChart.begin() + sortIndex);
		}
		renderChart.erase(renderChart.begin() + layerIndex);
	}

	PostDraw();
}

void ADXObject::PreDraw()
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(S_cmdList == nullptr);

	// コマンドリストをセット
	S_cmdList = ADXCommon::GetInstance()->GetCommandList();

	// ルートシグネチャの設定
	S_cmdList->SetGraphicsRootSignature(S_rootSignature.Get());
	// プリミティブ形状を設定
	S_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト


	ID3D12DescriptorHeap* srvHeap = ADXImage::GetSrvHeap();

	//SRVヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap };
	S_cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//SRVヒープの先頭にあるSRVをルートパラメーター1番に設定
	S_cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	S_GpuStartHandle = srvGpuHandle.ptr;

	// ルートシグネチャの設定コマンド
	S_cmdList->SetGraphicsRootSignature(S_rootSignature.Get());
}

void ADXObject::PostDraw()
{
	// コマンドリストを解除
	S_cmdList = nullptr;
	//ソート用配列を空にする
	S_allCameraPtr.clear();
}

void ADXObject::CreateRootSignature(D3D12_ROOT_SIGNATURE_DESC* rootSignatureDesc)
{
	HRESULT result = S_FALSE;
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	//ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	//ルートシグネチャの生成
	result = ADXCommon::GetInstance()->GetDevice()->
		CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&S_rootSignature));
	assert(SUCCEEDED(result));
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
	// nullptrチェック
	[[maybe_unused]] ID3D12Device* device = ADXCommon::GetInstance()->GetDevice();
	assert(device);
	assert(S_cmdList);

	//コンポーネントの描画前処理
	for (auto& itr : components_)
	{
		itr->OnWillRenderObject();
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

	//子オブジェクトも同様に消す
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