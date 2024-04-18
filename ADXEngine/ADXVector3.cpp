#include "ADXVector3.h"
#include <cmath>

using namespace DirectX;

ADXVector3::ADXVector3()
{
	x_ = 0;
	y_ = 0;
	z_ = 0;
}

ADXVector3::ADXVector3(float x, float y, float z)
{
	ADXVector3::x_ = x;
	ADXVector3::y_ = y;
	ADXVector3::z_ = z;
}

XMFLOAT3 ADXVector3::ConvertToXMFloat3() const
{
	XMFLOAT3 ret;
	ret = { x_,y_,z_ };
	return ret;
}

ADXVector3 ADXVector3::ConvertToADXVector3(XMFLOAT3 xf3)
{
	ADXVector3 ret;
	ret = { xf3.x,xf3.y,xf3.z };
	return ret;
}

float ADXVector3::Length() const
{
	return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

ADXVector3 ADXVector3::Normalize() const
{
	ADXVector3 ret = *this;
	float len = ret.Length();
	if (len != 0)
	{
		return ret /= len;
	}
	return ret;
}

float ADXVector3::Dot(const ADXVector3& v) const
{
	return x_ * v.x_ + y_ * v.y_ + z_ * v.z_;
}

ADXVector3 ADXVector3::Cross(const ADXVector3& v) const
{
	return ADXVector3(y_ * v.z_ - z_ * v.y_, z_ * v.x_ - x_ * v.z_, x_ * v.y_ - y_ * v.x_);
}

ADXVector3 ADXVector3::operator+() const
{
	return *this;
}

ADXVector3 ADXVector3::operator-() const
{
	return ADXVector3(-x_, -y_, -z_);
}

ADXVector3& ADXVector3::operator+=(const ADXVector3& v)
{
	x_ += v.x_;
	y_ += v.y_;
	z_ += v.z_;
	return *this;
}

ADXVector3& ADXVector3::operator-=(const ADXVector3& v)
{
	x_ -= v.x_;
	y_ -= v.y_;
	z_ -= v.z_;
	return *this;
}

ADXVector3& ADXVector3::operator*=(float s)
{
	x_ *= s;
	y_ *= s;
	z_ *= s;
	return *this;
}

ADXVector3& ADXVector3::operator/=(float s)
{
	x_ /= s;
	y_ /= s;
	z_ /= s;
	return *this;
}

bool ADXVector3::operator==(const ADXVector3& v)const
{
	return this->x_ == v.x_ && this->y_ == v.y_ && this->z_ == v.z_;
}

const ADXVector3 operator+(const ADXVector3& v1, const ADXVector3& v2)
{
	ADXVector3 temp(v1);
	return temp += v2;
}

const ADXVector3 operator-(const ADXVector3& v1, const ADXVector3& v2)
{
	ADXVector3 temp(v1);
	return temp -= v2;
}

const ADXVector3 operator*(const ADXVector3& v, float s)
{
	ADXVector3 temp(v);
	return temp *= s;
}

const ADXVector3 operator*(float s, const ADXVector3& v)
{
	ADXVector3 temp(v);
	return temp *= s;
}

const ADXVector3 operator/(const ADXVector3& v, float s)
{
	ADXVector3 temp(v);
	return temp /= s;
}