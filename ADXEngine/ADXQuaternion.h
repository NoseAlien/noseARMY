#pragma once

#include "ADXVector3.h"
#include "ADXMatrix4.h"

class ADXQuaternion
{
public:
	ADXVector3 v = ADXVector3{ 0,0,0 };
	float w = 1;

public:
	//����ADXQuaternion��Ԃ�
	ADXQuaternion Conjugate() const;
	//ADXQuaternion��norm��Ԃ�
	float Length();
	//���K������ADXQuaternion��Ԃ�
	ADXQuaternion Normalized() const;
	//�tADXQuaternion��Ԃ�
	ADXQuaternion Inverse() const;
	//ADXQuaternion�����]�s������߂�
	ADXMatrix4 RotateMatrix() const;

	ADXQuaternion operator-() const;

	ADXQuaternion& operator+=(const ADXQuaternion& q);
	ADXQuaternion& operator*=(float s);

public:
	//ADXQuaternion�̐�
	static ADXQuaternion Multiply(const ADXQuaternion& lhs, const ADXQuaternion& rhs);
	//�P��ADXQuaternion��Ԃ�
	static ADXQuaternion IdentityQuaternion();
	//�C�ӎ���]��\��ADXQuaternion�̐���
	static ADXQuaternion MakeAxisAngle(const ADXVector3& axis, float angle);
	//ZXY�I�C���[�p��Quaternion�ɕϊ�
	static ADXQuaternion EulerToQuaternion(const ADXVector3& eulerAngles);
	//Quaternion��ZXY�I�C���[�p�ɕϊ�
	static ADXVector3 QuaternionToEuler(const ADXQuaternion& quaternion);
	//�x�N�g����Quarternion�ŉ�]���������ʂ̃x�N�g�������߂�
	static ADXVector3 RotateVector(const ADXVector3& vector, const ADXQuaternion& quaternion);
	//����
	static float Dot(const ADXQuaternion& q0, const ADXQuaternion& q1);
	//���ʐ��`�⊮
	static ADXQuaternion Slerp(const ADXQuaternion& q0, const ADXQuaternion& q1, float t);
	//u����v�ւ̉�]�𐶐�
	static ADXQuaternion DirectionToDirection(const ADXVector3& u, const ADXVector3& v);
};

const ADXQuaternion operator+(const ADXQuaternion& q0, const ADXQuaternion& q1);
const ADXQuaternion operator*(const ADXQuaternion& q, float s);
const ADXQuaternion operator*(float s, const ADXQuaternion& q);
const ADXQuaternion operator*(const ADXQuaternion& q0, const ADXQuaternion& q1);