#pragma once

#include "ADXVector3.h"
#include "ADXMatrix4.h"
#include "ADXQuaternion.h"
#include <d3d12.h>
#include <wrl.h>
#include <list>

class ADXObject;

// �萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataTransform {
	ADXMatrix4 matWorld{};           // ���[�J�� �� ���[���h�ϊ��s��
	ADXMatrix4 matWorldRot{};           // ���[�J�� �� ���[���h�ϊ��s��i��]���̂݁j
	ADXMatrix4 matMVP{};             // ���[�J�� �� ���[���h �� �r���[�v���W�F�N�V�����ϊ��s��
	ADXVector3 cameraWorldPos{};     // �J�����̃��[���h���W
};

/// <summary>
/// ���[���h�ϊ��f�[�^
/// </summary>
class ADXWorldTransform {
public:
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;
	// �}�b�s���O�ς݃A�h���X
	ConstBufferDataTransform* constMapTransform_ = nullptr;

	// ���[�J�����W
	ADXVector3 localPosition_ = { 0, 0, 0 };
	// ���[�J����]��\���N�I�[�^�j�I��
	ADXQuaternion localRotation_ = ADXQuaternion::IdentityQuaternion();
	// ���[�J���X�P�[��
	ADXVector3 localScale_ = { 1, 1, 1 };
	// �e�ƂȂ郏�[���h�ϊ��ւ̃|�C���^
	ADXWorldTransform* parent_ = nullptr;
	// �X�v���C�g��`�悷�鎞�ȂǁA�J�����̈ʒu�≜�s���𖳎�����ꍇ�͂����true�ɂ���
	bool rectTransform_ = false;

	ADXVector3 modelPosition_ = { 0, 0, 0 };
	ADXQuaternion modelRotation_ = ADXQuaternion::IdentityQuaternion();
	ADXVector3 modelScale_ = { 1, 1, 1 };

private:
	// ����WorldTransform�����I�u�W�F�N�g
	ADXObject* gameObject_ = nullptr;
	// ���[�J�� �� ���[���h�ϊ��s��
	ADXMatrix4 matTrans_{};
	ADXMatrix4 matRot_{};
	ADXMatrix4 matScale_{};
	ADXMatrix4 matWorld_{};
	ADXMatrix4 matWorldInverse_{};

private:
	//�r���[�ϊ��s��
	static ADXMatrix4* S_matView;
	//�ˉe�ϊ��s��i�������e�j
	static ADXMatrix4* S_matProjection;

public:
	//������
	void Initialize(ADXObject* obj);

	//�s����X�V����
	void UpdateMatrix();

	//�萔�o�b�t�@���X�V����
	void UpdateConstBuffer();

	//���[���h���W���擾����
	ADXVector3 GetWorldPosition();

	//���[���h���W��������
	void SetWorldPosition(const ADXVector3& worldPos);

	//���[���h��]�p���擾����
	ADXQuaternion GetWorldRotation() const;

	//���[���h��]�p��������
	void SetWorldRotation(const ADXQuaternion& worldRot);

	ADXVector3 TransformPointWithoutTranslation(const ADXVector3& pos) const;

	ADXVector3 InverseTransformPointWithoutTranslation(const ADXVector3& pos) const;

	ADXVector3 TransformPointOnlyRotation(const ADXVector3& pos) const;

	ADXVector3 InverseTransformPointOnlyRotation(const ADXVector3& pos) const;

	ADXQuaternion TransformRotation(const ADXQuaternion& rot) const;

	ADXQuaternion InverseTransformRotation(const ADXQuaternion& rot) const;

	//���[���h�ϊ��s����擾����
	ADXMatrix4 GetMatWorld() { return matWorld_; };

	//���[���h�ϊ��s����擾����
	ADXMatrix4 GetMatWorldInverse();

	//���[���h��]�s����擾����
	ADXMatrix4 GetMatRot() { return matRot_; };

	//���[���h�X�P�[���s����擾����
	ADXMatrix4 GetMatScale() { return matScale_; };

	//����WorldTransform�����I�u�W�F�N�g���擾����
	ADXObject* GetGameObject() { return gameObject_; };

	//�q��S�Ď擾
	std::list<ADXWorldTransform*> GetChilds();

public:
	//�r���[�s��A�v���W�F�N�V�����s���������
	static void SetViewProjection(ADXMatrix4* matView, ADXMatrix4* matProjection);
	//�r���[�v���W�F�N�V�����s����擾����
	static ADXMatrix4 GetViewProjection();
	//�g�����X�t�H�[���s��𐶐�
	static ADXMatrix4 GenerateMatTransform(const ADXVector3& localPosition, const ADXQuaternion& localRotation, const ADXVector3& localScale);
};