#pragma once

#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include "ADXQuaternion.h"
#include <d3d12.h>
#include <wrl.h>

// �萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataTransform {
	ADXMatrix4 matWorld;           // ���[�J�� �� ���[���h�ϊ��s��
	ADXMatrix4 matWorldRot;           // ���[�J�� �� ���[���h�ϊ��s��i��]���̂݁j
	ADXMatrix4 matMVP;             // ���[�J�� �� ���[���h �� �r���[�v���W�F�N�V�����ϊ��s��
	ADXVector3 cameraWorldPos;     // �J�����̃��[���h���W
};

/// <summary>
/// ���[���h�ϊ��f�[�^
/// </summary>
class ADXWorldTransform {

public:
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	// �}�b�s���O�ς݃A�h���X
	ConstBufferDataTransform* constMapTransform = nullptr;
	// ���[�J�����W
	ADXVector3 localPosition_ = { 0, 0, 0 };
	// ���[�J����]��\���N�I�[�^�j�I��
	ADXQuaternion localRotation_ = ADXQuaternion::IdentityQuaternion();
	// ���[�J���X�P�[��
	ADXVector3 localScale_ = { 1, 1, 1 };
	// �e�ƂȂ郏�[���h�ϊ��ւ̃|�C���^
	ADXWorldTransform* parent_ = nullptr;
	// �X�v���C�g��`�悷�鎞�ȂǁA�J�����̈ʒu�≜�s���𖳎�����ꍇ�͂����true�ɂ���
	bool rectTransform = false;

	ADXVector3 modelPosition_ = { 0, 0, 0 };
	ADXQuaternion modelRotation_ = ADXQuaternion::IdentityQuaternion();
	ADXVector3 modelScale_ = { 1, 1, 1 };

private:
	// ���[�J�� �� ���[���h�ϊ��s��
	ADXMatrix4 matTrans_;
	ADXMatrix4 matRot_;
	ADXMatrix4 matScale_;
	ADXMatrix4 matWorld_;
	ADXMatrix4 matWorldInverse_;

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
	void SetWorldPosition(const ADXVector3& worldPos);

	ADXVector3 TransformPointWithoutTranslation(const ADXVector3& pos) const;

	ADXVector3 InverseTransformPointWithoutTranslation(const ADXVector3& pos) const;

	ADXVector3 TransformPointOnlyRotation(const ADXVector3& pos) const;

	ADXVector3 InverseTransformPointOnlyRotation(const ADXVector3& pos) const;

	/// <summary>
	///���[���h�ϊ��s����擾����
	/// </summary>
	ADXMatrix4 GetMatWorld() { return matWorld_; };

	/// <summary>
	///���[���h�ϊ��s����擾����
	/// </summary>
	ADXMatrix4 GetMatWorldInverse();

	/// <summary>
	///���[���h��]�s����擾����
	/// </summary>
	ADXMatrix4 GetMatRot() { return matRot_; };

	/// <summary>
	///���[���h�X�P�[���s����擾����
	/// </summary>
	ADXMatrix4 GetMatScale() { return matScale_; };

private:
	// �r���[�ϊ��s��
	static ADXMatrix4* S_matView;
	// �ˉe�ϊ��s��i�������e�j
	static ADXMatrix4* S_matProjection;

public:
	//�r���[�s��A�v���W�F�N�V�����s���������
	static void SetViewProjection(ADXMatrix4* matView, ADXMatrix4* matProjection);
	//�r���[�v���W�F�N�V�����s����擾����
	static ADXMatrix4 GetViewProjection();
	//�g�����X�t�H�[���s��𐶐�
	static ADXMatrix4 GenerateMatTransform(const ADXVector3& localPosition, const ADXQuaternion& localRotation, const ADXVector3& localScale);
};