#include "ADXVector2.h"
#include <cmath>

using namespace DirectX;

ADXVector2::ADXVector2()
{
	x_ = 0;
	y_ = 0;
}

ADXVector2::ADXVector2(float x, float y)
{
	ADXVector2::x_ = x;
	ADXVector2::y_ = y;
}

XMFLOAT2 ADXVector2::ConvertToXMFloat2()
{
	XMFLOAT2 ret;
	ret = { x_,y_ };
	return ret;
}

ADXVector2 ADXVector2::ConvertToADXVector2(XMFLOAT2 xf3)
{
	ADXVector2 ret;
	ret = { xf3.x,xf3.y };
	return ret;
}

float ADXVector2::Length() const
{
	return sqrt(x_ * x_ + y_ * y_);
}

ADXVector2 ADXVector2::Normalize() const
{
	ADXVector2 ret = *this;
	float len = ret.Length();
	if (len != 0)
	{
		return ret /= len;
	}
	return ret;
}

float ADXVector2::Dot(const ADXVector2& v) const
{
	return x_ * v.x_ + y_ * v.y_;
}

float ADXVector2::Cross(const ADXVector2& v) const
{
	return x_ * v.y_ - y_ * v.x_;
}

ADXVector2 ADXVector2::operator+() const
{
	return *this;
}

ADXVector2 ADXVector2::operator-() const
{
	return ADXVector2(-x_, -y_);
}

ADXVector2& ADXVector2::operator+=(const ADXVector2& v)
{
	x_ += v.x_;
	y_ += v.y_;
	return *this;
}

ADXVector2& ADXVector2::operator-=(const ADXVector2& v)
{
	x_ -= v.x_;
	y_ -= v.y_;
	return *this;
}

ADXVector2& ADXVector2::operator*=(float s)
{
	x_ *= s;
	y_ *= s;
	return *this;
}

ADXVector2& ADXVector2::operator/=(float s)
{
	x_ /= s;
	y_ /= s;
	return *this;
}

bool ADXVector2::operator==(const ADXVector2& v)const
{
	return this->x_ == v.x_ && this->y_ == v.y_;
}

const ADXVector2 operator+(const ADXVector2& v1, const ADXVector2& v2)
{
	ADXVector2 temp(v1);
	return temp += v2;
}

const ADXVector2 operator-(const ADXVector2& v1, const ADXVector2& v2)
{
	ADXVector2 temp(v1);
	return temp -= v2;
}

const ADXVector2 operator*(const ADXVector2& v, float s)
{
	ADXVector2 temp(v);
	return temp *= s;
}

const ADXVector2 operator*(float s, const ADXVector2& v)
{
	ADXVector2 temp(v);
	return temp *= s;
}

const ADXVector2 operator/(const ADXVector2& v, float s)
{
	ADXVector2 temp(v);
	return temp /= s;
}