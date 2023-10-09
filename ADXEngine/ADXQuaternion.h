#pragma once

#include "ADXVector3.h"
#include "ADXMatrix4.h"

class ADXQuaternion
{
public:
	ADXVector3 v = ADXVector3{ 0,0,0 };
	float w = 1;

public:
	//共役ADXQuaternionを返す
	ADXQuaternion Conjugate() const;
	//ADXQuaternionのnormを返す
	float Length();
	//正規化したADXQuaternionを返す
	ADXQuaternion Normalized() const;
	//逆ADXQuaternionを返す
	ADXQuaternion Inverse() const;
	//ADXQuaternionから回転行列を求める
	ADXMatrix4 RotateMatrix() const;

	ADXQuaternion operator-() const;

	ADXQuaternion& operator+=(const ADXQuaternion& q);
	ADXQuaternion& operator*=(float s);

public:
	//ADXQuaternionの積
	static ADXQuaternion Multiply(const ADXQuaternion& lhs, const ADXQuaternion& rhs);
	//単位ADXQuaternionを返す
	static ADXQuaternion IdentityQuaternion();
	//任意軸回転を表すADXQuaternionの生成
	static ADXQuaternion MakeAxisAngle(const ADXVector3& axis, float angle);
	//ZXYオイラー角をQuaternionに変換
	static ADXQuaternion EulerToQuaternion(const ADXVector3& eulerAngles);
	//QuaternionをZXYオイラー角に変換
	static ADXVector3 QuaternionToEuler(const ADXQuaternion& quaternion);
	//ベクトルをQuarternionで回転させた結果のベクトルを求める
	static ADXVector3 RotateVector(const ADXVector3& vector, const ADXQuaternion& quaternion);
	//内積
	static float Dot(const ADXQuaternion& q0, const ADXQuaternion& q1);
	//球面線形補完
	static ADXQuaternion Slerp(const ADXQuaternion& q0, const ADXQuaternion& q1, float t);
	//uからvへの回転を生成
	static ADXQuaternion DirectionToDirection(const ADXVector3& u, const ADXVector3& v);
};

const ADXQuaternion operator+(const ADXQuaternion& q0, const ADXQuaternion& q1);
const ADXQuaternion operator*(const ADXQuaternion& q, float s);
const ADXQuaternion operator*(float s, const ADXQuaternion& q);
const ADXQuaternion operator*(const ADXQuaternion& q0, const ADXQuaternion& q1);