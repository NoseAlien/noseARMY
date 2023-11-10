#pragma once

#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include "ADXQuaternion.h"
#include <d3d12.h>
#include <wrl.h>
#include <list>

class ADXObject;

// 定数バッファ用データ構造体
struct ConstBufferDataTransform {
	ADXMatrix4 matWorld{};           // ローカル → ワールド変換行列
	ADXMatrix4 matWorldRot{};           // ローカル → ワールド変換行列（回転情報のみ）
	ADXMatrix4 matMVP{};             // ローカル → ワールド → ビュープロジェクション変換行列
	ADXVector3 cameraWorldPos{};     // カメラのワールド座標
};

/// <summary>
/// ワールド変換データ
/// </summary>
class ADXWorldTransform {
public:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;
	// マッピング済みアドレス
	ConstBufferDataTransform* constMapTransform_ = nullptr;

	// ローカル座標
	ADXVector3 localPosition_ = { 0, 0, 0 };
	// ローカル回転を表すクオータニオン
	ADXQuaternion localRotation_ = ADXQuaternion::IdentityQuaternion();
	// ローカルスケール
	ADXVector3 localScale_ = { 1, 1, 1 };
	// 親となるワールド変換へのポインタ
	ADXWorldTransform* parent_ = nullptr;
	// スプライトを描画する時など、カメラの位置や奥行きを無視する場合はこれをtrueにする
	bool rectTransform_ = false;

	ADXVector3 modelPosition_ = { 0, 0, 0 };
	ADXQuaternion modelRotation_ = ADXQuaternion::IdentityQuaternion();
	ADXVector3 modelScale_ = { 1, 1, 1 };

private:
	// このWorldTransformを持つオブジェクト
	ADXObject* gameObject_ = nullptr;
	// ローカル → ワールド変換行列
	ADXMatrix4 matTrans_{};
	ADXMatrix4 matRot_{};
	ADXMatrix4 matScale_{};
	ADXMatrix4 matWorld_{};
	ADXMatrix4 matWorldInverse_{};

private:
	//ビュー変換行列
	static ADXMatrix4* S_matView;
	//射影変換行列（透視投影）
	static ADXMatrix4* S_matProjection;

public:
	//初期化
	void Initialize(ADXObject* obj);

	//行列を更新する
	void UpdateMatrix();

	//定数バッファを更新する
	void UpdateConstBuffer();

	//ワールド座標を取得する
	ADXVector3 GetWorldPosition();

	//ワールド座標を代入する
	void SetWorldPosition(const ADXVector3& worldPos);

	//ワールド回転角を取得する
	ADXQuaternion GetWorldRotation() const;

	//ワールド回転角を代入する
	void SetWorldRotation(const ADXQuaternion& worldRot);

	ADXVector3 TransformPointWithoutTranslation(const ADXVector3& pos) const;

	ADXVector3 InverseTransformPointWithoutTranslation(const ADXVector3& pos) const;

	ADXVector3 TransformPointOnlyRotation(const ADXVector3& pos) const;

	ADXVector3 InverseTransformPointOnlyRotation(const ADXVector3& pos) const;

	ADXQuaternion TransformRotation(const ADXQuaternion& rot) const;

	ADXQuaternion InverseTransformRotation(const ADXQuaternion& rot) const;

	//ワールド変換行列を取得する
	ADXMatrix4 GetMatWorld() { return matWorld_; };

	//ワールド変換行列を取得する
	ADXMatrix4 GetMatWorldInverse();

	//ワールド回転行列を取得する
	ADXMatrix4 GetMatRot() { return matRot_; };

	//ワールドスケール行列を取得する
	ADXMatrix4 GetMatScale() { return matScale_; };

	//このWorldTransformを持つオブジェクトを取得する
	ADXObject* GetGameObject() { return gameObject_; };

	//子を全て取得
	std::list<ADXWorldTransform*> GetChilds();

private:
	void CreateConstBuffer();

public:
	//ビュー行列、プロジェクション行列を代入する
	static void SetViewProjection(ADXMatrix4* matView, ADXMatrix4* matProjection);
	//ビュープロジェクション行列を取得する
	static ADXMatrix4 GetViewProjection();
	//トランスフォーム行列を生成
	static ADXMatrix4 GenerateMatTransform(const ADXVector3& localPosition, const ADXQuaternion& localRotation, const ADXVector3& localScale);
};