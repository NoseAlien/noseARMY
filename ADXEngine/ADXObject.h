#pragma once
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXImage.h"
#include "ADXComponentInclude.h"
#include "ADXUtility.h"
#include <string>
#include <memory>

class ADXCamera;

//ゲーム画面に出現するオブジェクト
class ADXObject
{
public:
	enum drawQuality
	{
		high,
		middle,
		low,
	};
	//定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataB0 {
		DirectX::XMMATRIX mat;
	};
	//ソート用
	struct sortingOrderGroup
	{
		int32_t sortingOrder_ = 0;
		std::vector<ADXObject*> objs_{};
	};
	struct renderLayerGroup
	{
		int32_t renderLayer_ = 0;
		std::vector<sortingOrderGroup> groups_{};
	};

public:
	static const FLOAT clearDepth;
	static const UINT8 clearStencil;

public:
	ADXWorldTransform transform_{};
	int32_t renderLayer_ = 0;
	int32_t sortingOrder_ = 0;
	bool alphaTex_ = false;
	bool isVisible_ = true;
	bool isActive_ = true;

private:
	std::list<std::unique_ptr<ADXComponent>> components_{};
	bool deleteFlag_ = false;

public:
	//このオブジェクトを描画
	void Draw();

	//このオブジェクトを次の更新処理の前に削除される状態にする
	void Destroy();

	//オブジェクトにコンポーネントを追加
	template <class Type>
	Type* AddComponent();

	//指定クラスのコンポーネントのうち配列の先頭にあるものを一つ取得
	template <class Type>
	Type* GetComponent();

	//指定クラスのコンポーネントを全て取得
	template <class Type>
	std::list<Type*> GetComponents();

	//持っているコライダーが別のコライダーに触れた時に呼ばれる
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);

	//このオブジェクトが次の更新処理の前に削除される状態ならtrueを返す
	bool GetDeleteFlag() { return deleteFlag_; };

private:
	//初期化処理
	void Initialize();

	//更新処理
	void Update();

private: // 静的メンバ変数
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> S_rootSignature;

	// デスクリプタサイズ
	static uint64_t S_descriptorHandleIncrementSize;

	// コマンドリスト
	static ID3D12GraphicsCommandList* S_cmdList;

	//SRVヒープの先頭ハンドル
	static uint64_t S_GpuStartHandle;

	// 全てのオブジェクトが入った配列
	static std::list<std::unique_ptr<ADXObject>> S_objs;

	// 全てのカメラを入れる配列
	static std::vector<ADXCamera*> S_allCameraPtr;

	// オブジェクトが存在できる領域を制限するための変数
	static ADXVector3 S_limitPos1;
	static ADXVector3 S_limitPos2;

	//描画（主にZソート）の精度
	static drawQuality S_drawQuality;

public: // 静的メンバ関数
	//静的初期化
	static void StaticInitialize();

	//静的更新処理
	static void StaticUpdate();

	// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

	// トランスフォーム用定数バッファ生成
	static void InitializeConstBufferTransform(ID3D12Resource** constBuff, ConstBufferDataTransform** constMap);

	// 全オブジェクトに対する描画処理
	static void StaticDraw();

	// ルートシグネチャを取得
	static ID3D12RootSignature* GetRootSignature() { return S_rootSignature.Get(); };

	// コマンドリストを取得
	static ID3D12GraphicsCommandList* GetCmdList() { return S_cmdList; };

	// SRVヒープの先頭ハンドルを取得
	static uint64_t GetGpuStartHandle() { return S_GpuStartHandle; };

	// ルートシグネチャの生成
	static void CreateRootSignature(D3D12_ROOT_SIGNATURE_DESC* rootSignatureDesc);

	// 全オブジェクトを取得
	static std::list<ADXObject*> GetObjs();

	// ワールド上の全てのカメラを入れておく配列に新しくカメラを入れる
	static void SetAllCameraPtr(ADXCamera* camPtr);

	//空のオブジェクトを生成
	static ADXObject* Create(const ADXVector3& setLocalPosition = { 0,0,0 },
		const ADXQuaternion& setLocalRotation = ADXQuaternion::IdentityQuaternion(),
		const ADXVector3& setLocalScale = { 1,1,1 }, ADXWorldTransform* setParent = nullptr);

	//既存のオブジェクトを複製
	static ADXObject* Duplicate(const ADXObject& prefab);

	//シーン切り替え時などに使用、今あるオブジェクトを全部消す
	static void Annihilate();

private:
	// 描画前処理
	static void PreDraw();

	// 描画後処理
	static void PostDraw();
};

template<class Type>
inline Type* ADXObject::AddComponent()
{
	Type* p = new Type();
	std::unique_ptr<ADXComponent> temp(p);
	temp->SetGameObject(this);
	components_.push_back(move(temp));
	return p;
}

template<class Type>
inline Type* ADXObject::GetComponent()
{
	if (this == nullptr)
	{
		return nullptr;
	}

	for (auto& itr : components_)
	{
		if (itr)
		{
			Type* p = dynamic_cast<Type*>(itr.get());

			if (p != nullptr)
			{
				return p;
			}
		}
	}
	return nullptr;
}

template<class Type>
inline std::list<Type*> ADXObject::GetComponents()
{
	if (this == nullptr)
	{
		return std::list<Type*>{};
	}

	std::list<Type*> ret = {};
	for (auto& itr : components_)
	{
		if (itr)
		{
			Type* p = dynamic_cast<Type*>(itr.get());

			if (p != nullptr)
			{
				ret.push_back(p);
			}
		}
	}
	return ret;
}