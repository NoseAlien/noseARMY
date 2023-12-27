#pragma once

#include <DirectXMath.h>

//3次元ベクトル
class ADXVector3
{
public:
	float x_;
	float y_;
	float z_;

public:
	//コンストラクタ
	ADXVector3();

	//コンストラクタ
	ADXVector3(float x, float y, float z);

	//XMFLOAT3に変換
	DirectX::XMFLOAT3 ConvertToXMFloat3();

	//長さを返す
	float Length() const;

	//正規化されたベクトルを返す
	ADXVector3 Normalize() const;

	//内積を返す
	float Dot(const ADXVector3& v) const;

	//外積を返す
	ADXVector3 Cross(const ADXVector3& v) const;

	ADXVector3 operator+() const;
	ADXVector3 operator-() const;

	ADXVector3& operator+=(const ADXVector3& v);
	ADXVector3& operator-=(const ADXVector3& v);
	ADXVector3& operator*=(float s);
	ADXVector3& operator/=(float s);

	bool operator==(const ADXVector3& v) const;

public:
	//XMFLOAT3から変換
	static ADXVector3 ConvertToADXVector3(DirectX::XMFLOAT3 xf3);
};

const ADXVector3 operator+(const ADXVector3& v1, const ADXVector3& v2);
const ADXVector3 operator-(const ADXVector3& v1, const ADXVector3& v2);
const ADXVector3 operator*(const ADXVector3& v, float s);
const ADXVector3 operator*(float s, const ADXVector3& v);
const ADXVector3 operator/(const ADXVector3& v, float s);