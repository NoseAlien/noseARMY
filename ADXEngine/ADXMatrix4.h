#pragma once

#include "ADXVector3.h"

/// <summary>
/// �s��
/// </summary>
class ADXMatrix4 {
public:
	// �sx��
	float m[4][4]{};

	// �R���X�g���N�^
	ADXMatrix4();
	// �������w�肵�Ă̐���
	ADXMatrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	DirectX::XMMATRIX ConvertToXMMatrix();

	ADXMatrix4 Inverse() const;

	ADXMatrix4 Transpose() const;

	// ������Z�q�I�[�o�[���[�h
	ADXMatrix4& operator*=(const ADXMatrix4& m2);

public:
	static ADXMatrix4 ConvertToADXMatrix(const DirectX::XMMATRIX& mat);

	//���W�ϊ��i�x�N�g���ƍs��̊|���Z������j
	static ADXVector3 Transform(const ADXVector3& v, const ADXMatrix4& m);
};

// 2�����Z�q�I�[�o�[���[�h
ADXMatrix4 operator*(const ADXMatrix4& m1, const ADXMatrix4& m2);

//�P�ʍs��
ADXMatrix4 IdentityMatrix();