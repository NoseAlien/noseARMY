#pragma once

#include "ADXVector3.h"

/// <summary>
/// 行列
/// </summary>
class ADXMatrix4 {
public:
	// 行x列
	float m_[4][4]{};

public:
	// コンストラクタ
	ADXMatrix4();
	// 成分を指定しての生成
	ADXMatrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	DirectX::XMMATRIX ConvertToXMMatrix();

	ADXMatrix4 Inverse() const;

	ADXMatrix4 Transpose() const;

	// 代入演算子オーバーロード
	ADXMatrix4& operator*=(const ADXMatrix4& m2);

public:
	static ADXMatrix4 ConvertToADXMatrix(const DirectX::XMMATRIX& mat);

	//座標変換（ベクトルと行列の掛け算をする）
	static ADXVector3 Transform(const ADXVector3& v, const ADXMatrix4& m);
};

// 2項演算子オーバーロード
ADXMatrix4 operator*(const ADXMatrix4& m1, const ADXMatrix4& m2);

//単位行列
ADXMatrix4 IdentityMatrix();