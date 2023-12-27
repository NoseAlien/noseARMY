#pragma once

#include <DirectXMath.h>

//2次元ベクトル
class ADXVector2
{
public:
	float x_;
	float y_;

public:
	//コンストラクタ
	ADXVector2();

	//コンストラクタ
	ADXVector2(float x, float y);

	//XMFLOAT2に変換
	DirectX::XMFLOAT2 ConvertToXMFloat2();

	//長さを返す
	float Length() const;

	//正規化されたベクトルを返す
	ADXVector2 Normalize() const;

	//内積を返す
	float Dot(const ADXVector2& v) const;

	//外積を返す
	float Cross(const ADXVector2& v) const;

	ADXVector2 operator+() const;
	ADXVector2 operator-() const;

	ADXVector2& operator+=(const ADXVector2& v);
	ADXVector2& operator-=(const ADXVector2& v);
	ADXVector2& operator*=(float s);
	ADXVector2& operator/=(float s);

	bool operator==(const ADXVector2& v) const;

public:
	//XMFLOAT2から変換
	static ADXVector2 ConvertToADXVector2(DirectX::XMFLOAT2 xf3);
};

const ADXVector2 operator+(const ADXVector2& v1, const ADXVector2& v2);
const ADXVector2 operator-(const ADXVector2& v1, const ADXVector2& v2);
const ADXVector2 operator*(const ADXVector2& v, float s);
const ADXVector2 operator*(float s, const ADXVector2& v);
const ADXVector2 operator/(const ADXVector2& v, float s);