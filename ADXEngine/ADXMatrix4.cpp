#include "ADXMatrix4.h"
#include <cassert>

using namespace DirectX;

ADXMatrix4::ADXMatrix4()
{
	IdentityMatrix();
}

ADXMatrix4::ADXMatrix4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	m_[0][0] = m00;
	m_[0][1] = m01;
	m_[0][2] = m02;
	m_[0][3] = m03;
	m_[1][0] = m10;
	m_[1][1] = m11;
	m_[1][2] = m12;
	m_[1][3] = m13;
	m_[2][0] = m20;
	m_[2][1] = m21;
	m_[2][2] = m22;
	m_[2][3] = m23;
	m_[3][0] = m30;
	m_[3][1] = m31;
	m_[3][2] = m32;
	m_[3][3] = m33;
}

XMMATRIX ADXMatrix4::ConvertToXMMatrix()
{
	XMMATRIX outPutXMM;
	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			outPutXMM.r[i].m128_f32[j] = m_[i][j];
		}
	}
	return outPutXMM;
}

ADXMatrix4 ADXMatrix4::ConvertToADXMatrix(const XMMATRIX& mat)
{
	ADXMatrix4 outPutADXM;
	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			outPutADXM.m_[i][j] = mat.r[i].m128_f32[j];
		}
	}
	return outPutADXM;
}

//�t�s��
ADXMatrix4 ADXMatrix4::Inverse() const
{
	const int32_t N = 4;

	ADXMatrix4 inv;

	double sweep[N][N * 2];

	double a; /* �萔�{�p */

	for (int32_t i = 0; i < N; i++) {
		for (int32_t j = 0; j < N; j++) {
			/* sweep�̍����ɋt�s������߂�s����Z�b�g */
			sweep[i][j] = m_[i][j];

			/* sweep�̉E���ɒP�ʍs����Z�b�g */
			sweep[i][N + j] = IdentityMatrix().m_[i][j];
		}
	}


	/* �S�Ă̗�̑Ίp�����ɑ΂���J��Ԃ� */
	for (int32_t k = 0; k < N; k++) {

		/* sweep[k][k]�Ɋ|�����1�ɂȂ�l�����߂� */
		a = 1 / sweep[k][k];

		/* ����i�Q�j�Fk�s�ڂ�a�{���� */
		for (int32_t j = 0; j < N * 2; j++) {
			/* ����ɂ��sweep[k][k]��1�ɂȂ� */
			sweep[k][j] *= a;
		}

		/* ����i�R�j�ɂ��k�s�ڈȊO�̍s��k��ڂ�0�ɂ��� */
		for (int32_t i = 0; i < N; i++) {
			if (i == k) {
				/* k�s�ڂ͂��̂܂� */
				continue;
			}

			/* k�s�ڂɊ|����l�����߂� */
			a = -sweep[i][k];

			for (int32_t j = 0; j < N * 2; j++) {
				/* i�s�ڂ�k�s�ڂ�a�{�����s�𑫂� */
				/* ����ɂ��sweep[i][k]��0�ɂȂ� */
				sweep[i][j] += sweep[k][j] * a;
			}
		}
	}

	/* sweep�̉E������mat�̋t�s�� */
	for (int32_t i = 0; i < N; i++) {
		for (int32_t j = 0; j < N; j++) {
			inv.m_[i][j] = (float)sweep[i][N + j];
		}
	}

	return inv;
}

ADXMatrix4 ADXMatrix4::Transpose() const
{
	const int32_t N = 4;

	ADXMatrix4 ret;

	for (int32_t i = 0; i < N; i++) {
		for (int32_t j = 0; j < N; j++) {
			ret.m_[i][j] = m_[j][i];
		}
	}

	return ret;
}

// ������Z�q�I�[�o�[���[�h
ADXMatrix4& ADXMatrix4::operator*=(const ADXMatrix4& m2)
{
	ADXMatrix4 m1;
	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			m1.m_[i][j] = m_[i][j];
		}
	}

	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			m_[i][j] = m1.m_[i][0] * m2.m_[0][j] + m1.m_[i][1] * m2.m_[1][j]
				+ m1.m_[i][2] * m2.m_[2][j] + m1.m_[i][3] * m2.m_[3][j];
		}
	}
	return *this;
}

//���W�ϊ��i�x�N�g���ƍs��̊|���Z������j
ADXVector3 ADXMatrix4::Transform(const ADXVector3& v, const ADXMatrix4& m)
{
	float w = v.x_ * m.m_[0][3] + v.y_ * m.m_[1][3] + v.z_ * m.m_[2][3] + m.m_[3][3];

	ADXVector3 result
	{
		(v.x_ * m.m_[0][0] + v.y_ * m.m_[1][0] + v.z_ * m.m_[2][0] + m.m_[3][0]) / w,
		(v.x_ * m.m_[0][1] + v.y_ * m.m_[1][1] + v.z_ * m.m_[2][1] + m.m_[3][1]) / w,
		(v.x_ * m.m_[0][2] + v.y_ * m.m_[1][2] + v.z_ * m.m_[2][2] + m.m_[3][2]) / w
	};
	return result;
}

// 2�����Z�q�I�[�o�[���[�h
ADXMatrix4 operator*(const ADXMatrix4& m1, const ADXMatrix4& m2)
{
	ADXMatrix4 result = m1;

	return result *= m2;
}

//�P�ʍs��
ADXMatrix4 IdentityMatrix()
{
	return
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1 };
}
