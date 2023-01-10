#pragma once

#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include <d3d12.h>
#include <wrl.h>

// �萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataTransform {
	ADXMatrix4 matWorld;           // ���[�J�� �� ���[���h�ϊ��s��
};

/// <summary>
/// ���[���h�ϊ��f�[�^
/// </summary>
class ADXWorldTransform {
private:
	// �r���[�ϊ��s��
	static ADXMatrix4* matView_;
	// �ˉe�ϊ��s��i�������e�j
	static ADXMatrix4* matProjection_;

public:
	static void SetViewProjection(ADXMatrix4* matView, ADXMatrix4* matProjection);

public:
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	// �}�b�s���O�ς݃A�h���X
	ConstBufferDataTransform* constMapTransform = nullptr;
	// ���[�J���X�P�[��
	ADXVector3 localScale_ = { 1, 1, 1 };
	// X,Y,Z�����̃��[�J���I�C���[�p
	ADXVector3 localEulerAngles_ = { 0, 0, 0 };
	// ���[�J�����W
	ADXVector3 localPosition_ = { 0, 0, 0 };
	// ���[�J�� �� ���[���h�ϊ��s��
	ADXMatrix4 matTrans_;
	ADXMatrix4 matRot_;
	ADXMatrix4 matScale_;
	ADXMatrix4 matWorld_;
	// �e�ƂȂ郏�[���h�ϊ��ւ̃|�C���^
	ADXWorldTransform* parent_ = nullptr;
	// �X�v���C�g��`�悷�鎞�ȂǁA�J�����̈ʒu�≜�s���𖳎�����ꍇ�͂����true�ɂ���
	bool rectTransform = false;

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �s����X�V����
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	///�萔�o�b�t�@���X�V����
	/// </summary>
	void UpdateConstBuffer();

	/// <summary>
	///���[���h���W���擾����
	/// </summary>
	ADXVector3 GetWorldPosition();

	/// <summary>
///���[���h���W��������
/// </summary>
	void SetWorldPosition(ADXVector3 worldPos);
};
