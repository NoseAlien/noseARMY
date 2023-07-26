#include "ADXQuaternion.h"
#include <cmath>
#include <limits>

ADXQuaternion ADXQuaternion::Conjugate() const
{
	ADXQuaternion ret = *this;

	ret.v = -ret.v;

	return ret;
}

float ADXQuaternion::Length()
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + w * w);
}

ADXQuaternion ADXQuaternion::Normalized()
{
	ADXQuaternion ret = *this;

	float len = ret.Length();
	ret = { ret.v / len,ret.w / len };

	return ret;
}

ADXQuaternion ADXQuaternion::Inverse()
{
	ADXQuaternion ret = *this;

	float len = ret.Length();
	ret = ret.Normalized().Conjugate();
	ret = { ret.v / len,ret.w / len };

	return ret;
}

ADXMatrix4 ADXQuaternion::RotateMatrix() const
{
	ADXMatrix4 ret
	{
		powf(w,2) + powf(v.x,2) - powf(v.y,2) - powf(v.z,2), 2 * (v.x * v.y + w * v.z), 2 * (v.x * v.z - w * v.y), 0,
		2 * (v.x * v.y - w * v.z), powf(w,2) - powf(v.x,2) + powf(v.y,2) - powf(v.z,2), 2 * (v.y * v.z + w * v.x), 0,
		2 * (v.x * v.z + w * v.y), 2 * (v.y * v.z - w * v.x), powf(w,2) - powf(v.x,2) - powf(v.y,2) + powf(v.z,2), 0,
		0,0,0,1
	};

	return ret;
}

ADXQuaternion ADXQuaternion::operator-() const
{
	return { -v,-w };
}

ADXQuaternion& ADXQuaternion::operator+=(const ADXQuaternion& q)
{
	*this = { v + q.v,w * q.w };
	return *this;
}

ADXQuaternion& ADXQuaternion::operator*=(float s)
{
	*this = { v * s,w * s };
	return *this;
}


ADXQuaternion ADXQuaternion::Multiply(const ADXQuaternion& lhs, const ADXQuaternion& rhs)
{
	ADXQuaternion ret;

	ret = { {
			(lhs.v.y * rhs.v.z) - (lhs.v.z * rhs.v.y) + (rhs.w * lhs.v.x) + (lhs.w * rhs.v.x),
			(lhs.v.z * rhs.v.x) - (lhs.v.x * rhs.v.z) + (rhs.w * lhs.v.y) + (lhs.w * rhs.v.y),
			(lhs.v.x * rhs.v.y) - (lhs.v.y * rhs.v.x) + (rhs.w * lhs.v.z) + (lhs.w * rhs.v.z)
		},
		(lhs.w * rhs.w) - (lhs.v.x * rhs.v.x) - (lhs.v.y * rhs.v.y) - (lhs.v.z * rhs.v.z)
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
			-cos(eulerAngles.x / 2) * sin(eulerAngles.y / 2) * sin(eulerAngles.z / 2) + sin(eulerAngles.x / 2) * cos(eulerAngles.y / 2) * cos(eulerAngles.z / 2),
			cos(eulerAngles.x / 2) * sin(eulerAngles.y / 2) * cos(eulerAngles.z / 2) + sin(eulerAngles.x / 2) * cos(eulerAngles.y / 2) * sin(eulerAngles.z / 2),
			sin(eulerAngles.x / 2) * sin(eulerAngles.y / 2) * cos(eulerAngles.z / 2) + cos(eulerAngles.x / 2) * cos(eulerAngles.y / 2) * sin(eulerAngles.z / 2)
		},
		-sin(eulerAngles.x / 2) * sin(eulerAngles.y / 2) * sin(eulerAngles.z / 2) + cos(eulerAngles.x / 2) * cos(eulerAngles.y / 2) * cos(eulerAngles.z / 2)
	};

	return ret;
}

ADXVector3 ADXQuaternion::QuaternionToEuler(const ADXQuaternion& quaternion)
{
	ADXVector3 ret = {};

	ret.x = asinf(2 * quaternion.v.y * quaternion.v.z + 2 * quaternion.v.x * quaternion.w);

	if (cos(ret.x) == 0)
	{
		ret.y = 0;

		ret.z = atan(((
			2 * quaternion.v.x * quaternion.v.y + 2 * quaternion.v.z * quaternion.w
			) /
			(
				2 * powf(quaternion.w, 2) + 2 * powf(quaternion.v.x, 2) - 1
				)));
	}
	else
	{
		ret.y = atan(-((
			2 * quaternion.v.x * quaternion.v.z - 2 * quaternion.v.y * quaternion.w
			) /
			(
				2 * powf(quaternion.w, 2) + 2 * powf(quaternion.v.z, 2) - 1
				)));

		ret.z = atan(-((
			2 * quaternion.v.x * quaternion.v.y - 2 * quaternion.v.z * quaternion.w
			) /
			(
				2 * powf(quaternion.w, 2) + 2 * powf(quaternion.v.y, 2) - 1
				)));
	}

	//double q0q0 = quaternion.w * quaternion.w;
	//double q0q1 = quaternion.w * quaternion.v.x;
	//double q0q2 = quaternion.w * quaternion.v.y;
	//double q0q3 = quaternion.w * quaternion.v.z;
	//double q1q1 = quaternion.v.x * quaternion.v.x;
	//double q1q2 = quaternion.v.x * quaternion.v.y;
	//double q1q3 = quaternion.v.x * quaternion.v.z;
	//double q2q2 = quaternion.v.y * quaternion.v.y;
	//double q2q3 = quaternion.v.y * quaternion.v.z;
	//double q3q3 = quaternion.v.z * quaternion.v.z;
	//ret.x = (float)atan2(2.0 * (q2q3 + q0q1), q0q0 - q1q1 - q2q2 + q3q3);
	//ret.y = (float)asin(2.0 * (q0q2 - q1q3));
	//ret.z = (float)atan2(2.0 * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3);

	return ret;
}

ADXVector3 ADXQuaternion::RotateVector(const ADXVector3& vector, const ADXQuaternion& quaternion)
{
	ADXQuaternion quateVec = { vector,0 };

	return Multiply(Multiply(quaternion, quateVec), quaternion.Conjugate()).v;
}

float ADXQuaternion::Dot(const ADXQuaternion& q0, const ADXQuaternion& q1)
{
	return q0.v.x * q1.v.x + q0.v.y * q1.v.y + q0.v.z * q1.v.z + q0.w * q1.w;
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
				(1.0f - t) * q0.v.x + t * q1.v.x,
				(1.0f - t) * q0.v.y + t * q1.v.y,
				(1.0f - t) * q0.v.z + t * q1.v.z,
			},
			(1.0f - t) * q0.w + t * q1.w
		};

		return ret;
	}

	if (dot < 0)
	{
		q0f = -q0f;
		dot = -dot;
	}
	//‚È‚·Šp‚ð‹‚ß‚é
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
			scale0 * q0.v.x + scale1 * q1.v.x,
			scale0 * q0.v.y + scale1 * q1.v.y,
			scale0 * q0.v.z + scale1 * q1.v.z,
		},
		scale0 * q0.w + scale1 * q1.w
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