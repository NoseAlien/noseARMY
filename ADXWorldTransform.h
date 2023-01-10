#pragma once

#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体
struct ConstBufferDataTransform {
	ADXMatrix4 matWorld;           // ローカル → ワールド変換行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
class ADXWorldTransform {
private:
	// ビュー変換行列
	static ADXMatrix4* matView_;
	// 射影変換行列（透視投影）
	static ADXMatrix4* matProjection_;

public:
	static void SetViewProjection(ADXMatrix4* matView, ADXMatrix4* matProjection);

public:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	// マッピング済みアドレス
	ConstBufferDataTransform* constMapTransform = nullptr;
	// ローカルスケール
	ADXVector3 localScale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカルオイラー角
	ADXVector3 localEulerAngles_ = { 0, 0, 0 };
	// ローカル座標
	ADXVector3 localPosition_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	ADXMatrix4 matTrans_;
	ADXMatrix4 matRot_;
	ADXMatrix4 matScale_;
	ADXMatrix4 matWorld_;
	// 親となるワールド変換へのポインタ
	ADXWorldTransform* parent_ = nullptr;
	// スプライトを描画する時など、カメラの位置や奥行きを無視する場合はこれをtrueにする
	bool rectTransform = false;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	///定数バッファを更新する
	/// </summary>
	void UpdateConstBuffer();

	/// <summary>
	///ワールド座標を取得する
	/// </summary>
	ADXVector3 GetWorldPosition();

	/// <summary>
///ワールド座標を代入する
/// </summary>
	void SetWorldPosition(ADXVector3 worldPos);
};
