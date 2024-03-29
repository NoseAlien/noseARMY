﻿#include "ADXMatrix4.h"
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

ADXMatrix4 ADXMatrix4::Inverse() const
{
	const int32_t N = 4;

	ADXMatrix4 inv;

	double sweep[N][N * 2];

	double a; /* 定数倍用 */

	for (int32_t i = 0; i < N; i++) {
		for (int32_t j = 0; j < N; j++) {
			/* sweepの左側に逆行列を求める行列をセット */
			sweep[i][j] = m_[i][j];

			/* sweepの右側に単位行列をセット */
			sweep[i][N + j] = IdentityMatrix().m_[i][j];
		}
	}


	/* 全ての列の対角成分に対する繰り返し */
	for (int32_t k = 0; k < N; k++) {

		/* sweep[k][k]に掛けると1になる値を求める */
		a = 1 / sweep[k][k];

		/* 操作（２）：k行目をa倍する */
		for (int32_t j = 0; j < N * 2; j++) {
			/* これによりsweep[k][k]が1になる */
			sweep[k][j] *= a;
		}

		/* 操作（３）によりk行目以外の行のk列目を0にする */
		for (int32_t i = 0; i < N; i++) {
			if (i == k) {
				/* k行目はそのまま */
				continue;
			}

			/* k行目に掛ける値を求める */
			a = -sweep[i][k];

			for (int32_t j = 0; j < N * 2; j++) {
				/* i行目にk行目をa倍した行を足す */
				/* これによりsweep[i][k]が0になる */
				sweep[i][j] += sweep[k][j] * a;
			}
		}
	}

	/* sweepの右半分がmatの逆行列 */
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

ADXVector3 ADXMatrix4::Transform(const ADXVector3& v, const ADXMatrix4& m)
{
	float w = v.x_ * m.m_[0][3] + v.y_ * m.m_[1][3] + v.z_ * m.m_[2][3] + m.m_[3][3];

	//ベクトルと行列の掛け算の更新
	ADXVector3 result
	{
		(v.x_ * m.m_[0][0] + v.y_ * m.m_[1][0] + v.z_ * m.m_[2][0] + m.m_[3][0]) / w,
		(v.x_ * m.m_[0][1] + v.y_ * m.m_[1][1] + v.z_ * m.m_[2][1] + m.m_[3][1]) / w,
		(v.x_ * m.m_[0][2] + v.y_ * m.m_[1][2] + v.z_ * m.m_[2][2] + m.m_[3][2]) / w
	};
	return result;
}

ADXMatrix4 operator*(const ADXMatrix4& m1, const ADXMatrix4& m2)
{
	ADXMatrix4 result = m1;

	return result *= m2;
}

ADXMatrix4 IdentityMatrix()
{
	//単位行列を返す
	return
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1 };
}
