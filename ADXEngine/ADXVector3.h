#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class ADXVector3
{
public:
	float x;
	float y;
	float z;

public:
	ADXVector3();
	ADXVector3(float x, float y, float z);

	XMFLOAT3 ConvertToXMFloat3();

	float length() const;
	ADXVector3& normalize();
	static ADXVector3 normalized(const ADXVector3& v);
	float dot(const ADXVector3& v) const;
	ADXVector3 cross(const ADXVector3& v) const;

	ADXVector3 operator+() const;
	ADXVector3 operator-() const;

	ADXVector3& operator+=(const ADXVector3& v);
	ADXVector3& operator-=(const ADXVector3& v);
	ADXVector3& operator*=(float s);
	ADXVector3& operator/=(float s);

	bool operator==(const ADXVector3& v) const;

public:
	static ADXVector3 ConvertToADXVector3(XMFLOAT3 xf3);
};

const ADXVector3 operator+(const ADXVector3& v1, const ADXVector3& v2);
const ADXVector3 operator-(const ADXVector3& v1, const ADXVector3& v2);
const ADXVector3 operator*(const ADXVector3& v, float s);
const ADXVector3 operator*(float s, const ADXVector3& v);
const ADXVector3 operator/(const ADXVector3& v, float s);