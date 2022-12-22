#pragma once
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXImage.h"
#include "ADXComponentInclude.h"
#include <string>

#pragma comment(lib, "d3dcompiler.lib")

class ADXObject
{
public:

	//定数バッファ用データ構造体（マテリアル）

	struct ConstBufferDataB0 {
		XMMATRIX mat;
	};

	struct ConstBufferDataB1 {
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

public:
	ADXObject();
	void Initialize();
	void CreateConstBuffer();
	void Update();
	void Draw(UINT64 GpuStartHandle);

protected:
	virtual void UniqueUpdate();

public:
	ADXWorldTransform transform{};
	ADXModel* model = nullptr;
	ADXMaterial material{};
	ADXImage texture{};
	std::vector<ADXCollider> colliders{};
	int renderLayer = 0;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1 = nullptr;

	ConstBufferDataB0* constMapMaterial = nullptr;

public: // 静的メンバ関数
	//静的初期化
	static void StaticInitialize(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);

	//静的更新処理
	static void StaticUpdate();

	// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

	// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	// 描画処理
	static void StaticDraw(ID3D12DescriptorHeap* srvHeap);

	// 描画後処理
	static void PostDraw();

	static std::vector<ADXObject*> GetAllObjs();

	static ADXObject Duplicate(ADXObject prefab);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// デスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	static Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	static Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	static D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle;
	// ビュー行列
	static ADXMatrix4 matView;
	// 射影行列
	static ADXMatrix4 matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// 全てのオブジェクトを入れる配列
	static std::vector<ADXObject*> allObjPtr;
	// 全てのオブジェクトが入った配列
	static std::vector<ADXObject*> objs;
};