#pragma once
#pragma once

#include <DirectXMath.h>

class ADXVector2
{
public:
	float x_;
	float y_;

public:
	ADXVector2();
	ADXVector2(float x, float y);

	DirectX::XMFLOAT2 ConvertToXMFloat2();

	float Length() const;
	ADXVector2 Normalize() const;
	float Dot(const ADXVector2& v) const;
	float Cross(const ADXVector2& v) const;

	ADXVector2 operator+() const;
	ADXVector2 operator-() const;

	ADXVector2& operator+=(const ADXVector2& v);
	ADXVector2& operator-=(const ADXVector2& v);
	ADXVector2& operator*=(float s);
	ADXVector2& operator/=(float s);

	bool operator==(const ADXVector2& v) const;

public:
	static ADXVector2 ConvertToADXVector2(DirectX::XMFLOAT2 xf3);
};

const ADXVector2 operator+(const ADXVector2& v1, const ADXVector2& v2);
const ADXVector2 operator-(const ADXVector2& v1, const ADXVector2& v2);
const ADXVector2 operator*(const ADXVector2& v, float s);
const ADXVector2 operator*(float s, const ADXVector2& v);
const ADXVector2 operator/(const ADXVector2& v, float s);