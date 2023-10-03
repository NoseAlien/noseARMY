#pragma once
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXImage.h"
#include "ADXComponentInclude.h"
#include "ADXUtility.h"
#include <string>
#include <memory>

class ADXCamera;

class ADXObject
{
public:

	//定数バッファ用データ構造体（マテリアル）

	struct ConstBufferDataB0 {
		DirectX::XMMATRIX mat;
	};

	struct ConstBufferDataB1 {
		DirectX::XMFLOAT3 ambient;
		float pad1;
		DirectX::XMFLOAT3 diffuse;
		float pad2;
		DirectX::XMFLOAT3 specular;
		float alpha;
	};

public:
	void Draw();
	void Destroy();
	template <class Type>
	Type* AddComponent();
	template <class Type>
	Type* GetComponent();
	template <class Type>
	std::list<Type*> GetComponents();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);

protected:
	virtual void UniqueUpdate() {};
	virtual void OnPreRender() {};
	virtual void OnWillRenderObject() {};
	virtual void Rendered() {};

private:
	void Initialize();
	void Update();
	void CreateConstBuffer();

public:
	ADXWorldTransform transform{};
	ADXModel* model = nullptr;
	ADXMaterial material{};
	uint32_t texture = 0;
	int32_t renderLayer = 0;
	int32_t sortingOrder = 0;
	bool alphaTex = false;
	bool isVisible = true;
	bool isActive = true;
	bool useDefaultDraw = true;

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1 = nullptr;
	ConstBufferDataB0* constMapMaterial = nullptr;

private:
	std::list<std::unique_ptr<ADXComponent, ADXUtility::NPManager<ADXComponent>>> components{};
	bool deleteFlag = false;

public: // 静的メンバ関数
	//静的初期化
	static void StaticInitialize();
	//静的更新処理
	static void StaticUpdate();
	// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();
	// 描画前処理
	static void PreDraw();
	// 描画処理
	static void StaticDraw();
	// 描画後処理
	static void PostDraw();

	static std::list<ADXObject*> GetObjs();

	static void SetAllCameraPtr(ADXCamera* camPtr);

	//空のオブジェクトを生成
	static ADXObject* Create(const ADXVector3& setLocalPosition = { 0,0,0 },
		const ADXQuaternion& setLocalRotation = ADXQuaternion::IdentityQuaternion(),
		const ADXVector3& setLocalScale = { 1,1,1 }, ADXWorldTransform* parent = nullptr);

	//既存のオブジェクトを複製
	static ADXObject* Duplicate(const ADXObject& prefab);

private: // 静的メンバ変数
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> S_rootSignature;
	// パイプラインステートオブジェクト（不透明オブジェクト用）
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> S_pipelineState;
	// パイプラインステートオブジェクト（半透明オブジェクト用）
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> S_pipelineStateAlpha;
	// デスクリプタサイズ
	static uint64_t S_descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* S_cmdList;
	// シェーダリソースビューのハンドル(CPU)
	static D3D12_CPU_DESCRIPTOR_HANDLE S_cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;

	static uint64_t S_GpuStartHandle;
	// 全てのオブジェクトが入った配列
	static std::list<std::unique_ptr<ADXObject, ADXUtility::NPManager<ADXObject>>> S_objs;
	// 全てのカメラを入れる配列
	static std::vector<ADXCamera*> S_allCameraPtr;
	// オブジェクトが存在できる領域を制限するための変数
	static ADXVector3 S_limitPos1;
	static ADXVector3 S_limitPos2;
	
	static bool S_highQualityZSort;
};

template<class Type>
inline Type* ADXObject::AddComponent()
{
	std::unique_ptr<ADXComponent, ADXUtility::NPManager<ADXComponent>> temp(new Type);
	components.push_back(move(temp));
	return components.back().get();
}

template<class Type>
inline Type* ADXObject::GetComponent()
{
	for (auto& itr : components)
	{
		if (dynamic_cast<Type*>(itr.get()))
		{
			return itr.get();
		}
	}
	return nullptr;
}

template<class Type>
inline std::list<Type*> ADXObject::GetComponents()
{
	std::list<Type*> ret = {};
	for (auto& itr : components)
	{
		if (dynamic_cast<Type*>(itr.get()))
		{
			ret.push_back(itr.get());
		}
	}
	return ret;
}
