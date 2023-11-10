#include "ADXQuaternion.h"
#include <cmath>
#include <limits>

ADXQuaternion ADXQuaternion::Conjugate() const
{
	ADXQuaternion ret = *this;

	ret.v_ = -ret.v_;

	return ret;
}

float ADXQuaternion::Length()
{
	return sqrt(v_.x_ * v_.x_ + v_.y_ * v_.y_ + v_.z_ * v_.z_ + w_ * w_);
}

ADXQuaternion ADXQuaternion::Normalized() const
{
	ADXQuaternion ret = *this;

	float len = ret.Length();
	ret = { ret.v_ / len,ret.w_ / len };

	return ret;
}

ADXQuaternion ADXQuaternion::Inverse() const
{
	ADXQuaternion ret = *this;

	float len = ret.Length();
	ret = ret.Normalized().Conjugate();
	ret = { ret.v_ / len,ret.w_ / len };

	return ret;
}

ADXMatrix4 ADXQuaternion::RotateMatrix() const
{
	ADXMatrix4 ret
	{
		powf(w_,2) + powf(v_.x_,2) - powf(v_.y_,2) - powf(v_.z_,2), 2 * (v_.x_ * v_.y_ + w_ * v_.z_), 2 * (v_.x_ * v_.z_ - w_ * v_.y_), 0,
		2 * (v_.x_ * v_.y_ - w_ * v_.z_), powf(w_,2) - powf(v_.x_,2) + powf(v_.y_,2) - powf(v_.z_,2), 2 * (v_.y_ * v_.z_ + w_ * v_.x_), 0,
		2 * (v_.x_ * v_.z_ + w_ * v_.y_), 2 * (v_.y_ * v_.z_ - w_ * v_.x_), powf(w_,2) - powf(v_.x_,2) - powf(v_.y_,2) + powf(v_.z_,2), 0,
		0,0,0,1
	};

	return ret;
}

ADXQuaternion ADXQuaternion::operator-() const
{
	return { -v_,-w_ };
}

ADXQuaternion& ADXQuaternion::operator+=(const ADXQuaternion& q)
{
	*this = { v_ + q.v_,w_ * q.w_ };
	return *this;
}

ADXQuaternion& ADXQuaternion::operator*=(float s)
{
	*this = { v_ * s,w_ * s };
	return *this;
}


ADXQuaternion ADXQuaternion::Multiply(const ADXQuaternion& lhs, const ADXQuaternion& rhs)
{
	ADXQuaternion ret;

	ret = { {
			(lhs.v_.y_ * rhs.v_.z_) - (lhs.v_.z_ * rhs.v_.y_) + (rhs.w_ * lhs.v_.x_) + (lhs.w_ * rhs.v_.x_),
			(lhs.v_.z_ * rhs.v_.x_) - (lhs.v_.x_ * rhs.v_.z_) + (rhs.w_ * lhs.v_.y_) + (lhs.w_ * rhs.v_.y_),
			(lhs.v_.x_ * rhs.v_.y_) - (lhs.v_.y_ * rhs.v_.x_) + (rhs.w_ * lhs.v_.z_) + (lhs.w_ * rhs.v_.z_)
		},
		(lhs.w_ * rhs.w_) - (lhs.v_.x_ * rhs.v_.x_) - (lhs.v_.y_ * rhs.v_.y_) - (lhs.v_.z_ * rhs.v_.z_)
	};

	return ret;
}

ADXQuaternion ADXQuaternion::IdentityQuaternion()
{
	return { {0.0f,0.0f,0.0f},1.0f };
}

ADXQuaternion ADXQuaternion::MakeAxisAngle(const ADXVector3& axis, float angle)
{
	ADXQuaternion ret;
	ADXVector3 normalizedAxis = axis;
	normalizedAxis = normalizedAxis.Normalize();

	ret = { normalizedAxis * sin(angle / 2.0f),cos(angle / 2.0f) };

	return ret;
}

ADXQuaternion ADXQuaternion::EulerToQuaternion(const ADXVector3& eulerAngles)
{
	ADXQuaternion ret = {
		{
			-cos(eulerAngles.x_ / 2) * sin(eulerAngles.y_ / 2) * sin(eulerAngles.z_ / 2) + sin(eulerAngles.x_ / 2) * cos(eulerAngles.y_ / 2) * cos(eulerAngles.z_ / 2),
			cos(eulerAngles.x_ / 2) * sin(eulerAngles.y_ / 2) * cos(eulerAngles.z_ / 2) + sin(eulerAngles.x_ / 2) * cos(eulerAngles.y_ / 2) * sin(eulerAngles.z_ / 2),
			sin(eulerAngles.x_ / 2) * sin(eulerAngles.y_ / 2) * cos(eulerAngles.z_ / 2) + cos(eulerAngles.x_ / 2) * cos(eulerAngles.y_ / 2) * sin(eulerAngles.z_ / 2)
		},
		-sin(eulerAngles.x_ / 2) * sin(eulerAngles.y_ / 2) * sin(eulerAngles.z_ / 2) + cos(eulerAngles.x_ / 2) * cos(eulerAngles.y_ / 2) * cos(eulerAngles.z_ / 2)
	};

	return ret;
}

ADXVector3 ADXQuaternion::QuaternionToEuler(const ADXQuaternion& quaternion)
{
	ADXVector3 ret = {};

	ret.x_ = asinf(2 * quaternion.v_.y_ * quaternion.v_.z_ + 2 * quaternion.v_.x_ * quaternion.w_);

	if (cos(ret.x_) == 0)
	{
		ret.y_ = 0;

		ret.z_ = atan(((
			2 * quaternion.v_.x_ * quaternion.v_.y_ + 2 * quaternion.v_.z_ * quaternion.w_
			) /
			(
				2 * powf(quaternion.w_, 2) + 2 * powf(quaternion.v_.x_, 2) - 1
				)));
	}
	else
	{
		ret.y_ = atan(-((
			2 * quaternion.v_.x_ * quaternion.v_.z_ - 2 * quaternion.v_.y_ * quaternion.w_
			) /
			(
				2 * powf(quaternion.w_, 2) + 2 * powf(quaternion.v_.z_, 2) - 1
				)));

		ret.z_ = atan(-((
			2 * quaternion.v_.x_ * quaternion.v_.y_ - 2 * quaternion.v_.z_ * quaternion.w_
			) /
			(
				2 * powf(quaternion.w_, 2) + 2 * powf(quaternion.v_.y_, 2) - 1
				)));
	}

	return ret;
}

ADXVector3 ADXQuaternion::RotateVector(const ADXVector3& vector, const ADXQuaternion& quaternion)
{
	ADXQuaternion quateVec = { vector,0 };

	return Multiply(Multiply(quaternion, quateVec), quaternion.Conjugate()).v_;
}

float ADXQuaternion::Dot(const ADXQuaternion& q0, const ADXQuaternion& q1)
{
	return q0.v_.x_ * q1.v_.x_ + q0.v_.y_ * q1.v_.y_ + q0.v_.z_ * q1.v_.z_ + q0.w_ * q1.w_;
}

ADXQuaternion ADXQuaternion::Slerp(const ADXQuaternion& q0, const ADXQuaternion& q1, float t)
{
	ADXQuaternion q0f = q0;
	ADXQuaternion q1f = q1;

	float dot = Dot(q0f, q1f);

	if (dot >= 1.0f - FLT_EPSILON)
	{
		ADXQuaternion ret =
		{
			{
				(1.0f - t) * q0.v_.x_ + t * q1.v_.x_,
				(1.0f - t) * q0.v_.y_ + t * q1.v_.y_,
				(1.0f - t) * q0.v_.z_ + t * q1.v_.z_,
			},
			(1.0f - t) * q0.w_ + t * q1.w_
		};

		return ret;
	}

	if (dot < 0)
	{
		q0f = -q0f;
		dot = -dot;
	}
	//なす角を求める
	float theta = std::acosf(dot);

	float st = std::sinf(theta);

	if (st == 0)
	{
		return q0f;
	}

	float sut = std::sinf(t * theta);
	float sout = std::sinf((1 - t) * theta);

	float scale0 = sout / st;
	float scale1 = sut / st;

	ADXQuaternion ret =
	{
		{
			scale0 * q0.v_.x_ + scale1 * q1.v_.x_,
			scale0 * q0.v_.y_ + scale1 * q1.v_.y_,
			scale0 * q0.v_.z_ + scale1 * q1.v_.z_,
		},
		scale0 * q0.w_ + scale1 * q1.w_
	};

	return ret;
}

ADXQuaternion ADXQuaternion::DirectionToDirection(const ADXVector3& u, const ADXVector3& v)
{
	ADXVector3 v0 = u;
	v0 = v0.Normalize();

	ADXVector3 v1 = v;
	v1 = v0.Normalize();
	float dot = v0.Dot(v1);

	ADXVector3 cross = v0.Cross(v1);

	ADXVector3 axis = cross.Normalize();

	float theta = std::acos(dot);

	return MakeAxisAngle(axis, theta);
}

const ADXQuaternion operator+(const ADXQuaternion& q0, const ADXQuaternion& q1)
{
	ADXQuaternion temp(q0);
	return temp += q1;
}

const ADXQuaternion operator*(const ADXQuaternion& q, float s)
{
	ADXQuaternion temp(q);
	return temp *= s;
}

const ADXQuaternion operator*(float s, const ADXQuaternion& q)
{
	ADXQuaternion temp(q);
	return temp *= s;
}

const ADXQuaternion operator*(const ADXQuaternion& q0, const ADXQuaternion& q1)
{
	return ADXQuaternion::Multiply(q0, q1);
}
