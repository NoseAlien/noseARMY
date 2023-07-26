#pragma once

#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include "ADXQuaternion.h"
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体
struct ConstBufferDataTransform {
	ADXMatrix4 matWorld;           // ローカル → ワールド変換行列
	ADXMatrix4 matWorldRot;           // ローカル → ワールド変換行列（回転情報のみ）
	ADXMatrix4 matMVP;             // ローカル → ワールド → ビュープロジェクション変換行列
	ADXVector3 cameraWorldPos;     // カメラのワールド座標
};

/// <summary>
/// ワールド変換データ
/// </summary>
class ADXWorldTransform {

public:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	// マッピング済みアドレス
	ConstBufferDataTransform* constMapTransform = nullptr;
	// ローカル座標
	ADXVector3 localPosition_ = { 0, 0, 0 };
	// ローカル回転を表すクオータニオン
	ADXQuaternion localRotation_ = ADXQuaternion::IdentityQuaternion();
	// ローカルスケール
	ADXVector3 localScale_ = { 1, 1, 1 };
	// 親となるワールド変換へのポインタ
	ADXWorldTransform* parent_ = nullptr;
	// スプライトを描画する時など、カメラの位置や奥行きを無視する場合はこれをtrueにする
	bool rectTransform = false;

	ADXVector3 modelPosition_ = { 0, 0, 0 };
	ADXQuaternion modelRotation_ = ADXQuaternion::IdentityQuaternion();
	ADXVector3 modelScale_ = { 1, 1, 1 };

private:
	// ローカル → ワールド変換行列
	ADXMatrix4 matTrans_;
	ADXMatrix4 matRot_;
	ADXMatrix4 matScale_;
	ADXMatrix4 matWorld_;
	ADXMatrix4 matWorldInverse_;

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
	void SetWorldPosition(const ADXVector3& worldPos);

	ADXVector3 TransformPointWithoutTranslation(const ADXVector3& pos) const;

	ADXVector3 InverseTransformPointWithoutTranslation(const ADXVector3& pos) const;

	ADXVector3 TransformPointOnlyRotation(const ADXVector3& pos) const;

	ADXVector3 InverseTransformPointOnlyRotation(const ADXVector3& pos) const;

	/// <summary>
	///ワールド変換行列を取得する
	/// </summary>
	ADXMatrix4 GetMatWorld() { return matWorld_; };

	/// <summary>
	///ワールド変換行列を取得する
	/// </summary>
	ADXMatrix4 GetMatWorldInverse();

	/// <summary>
	///ワールド回転行列を取得する
	/// </summary>
	ADXMatrix4 GetMatRot() { return matRot_; };

	/// <summary>
	///ワールドスケール行列を取得する
	/// </summary>
	ADXMatrix4 GetMatScale() { return matScale_; };

private:
	// ビュー変換行列
	static ADXMatrix4* S_matView;
	// 射影変換行列（透視投影）
	static ADXMatrix4* S_matProjection;

public:
	//ビュー行列、プロジェクション行列を代入する
	static void SetViewProjection(ADXMatrix4* matView, ADXMatrix4* matProjection);
	//ビュープロジェクション行列を取得する
	static ADXMatrix4 GetViewProjection();
	//トランスフォーム行列を生成
	static ADXMatrix4 GenerateMatTransform(const ADXVector3& localPosition, const ADXQuaternion& localRotation, const ADXVector3& localScale);
};