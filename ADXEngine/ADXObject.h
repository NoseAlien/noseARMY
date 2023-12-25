#pragma once
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXImage.h"
#include "ADXComponentInclude.h"
#include "ADXUtility.h"
#include <string>
#include <memory>

class ADXCamera;

class ADXObject
{
public:
	//�萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
	struct ConstBufferDataB0 {
		DirectX::XMMATRIX mat;
	};

	struct ConstBufferDataB1 {
		DirectX::XMFLOAT3 ambient;
		float pad1;
		DirectX::XMFLOAT3 diffuse;
		float pad2;
		DirectX::XMFLOAT3 specular;
		float alpha;
	};

public:
	ADXWorldTransform transform_{};
	ADXModel* model_ = nullptr;
	ADXMaterial material_{};
	uint32_t texture_ = 0;
	int32_t renderLayer_ = 0;
	int32_t sortingOrder_ = 0;
	bool alphaTex_ = false;
	bool isVisible_ = true;
	bool isActive_ = true;
	bool useDefaultDraw_ = true;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1_ = nullptr;
	std::list<std::unique_ptr<ADXComponent, ADXUtility::NPManager<ADXComponent>>> components_{};
	bool deleteFlag_ = false;

private: // �ÓI�����o�ϐ�
	// ���[�g�V�O�l�`��
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> S_rootSignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g�i�s�����I�u�W�F�N�g�p�j
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> S_pipelineState;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g�i�������I�u�W�F�N�g�p�j
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> S_pipelineStateAlpha;
	// �f�X�N���v�^�T�C�Y
	static uint64_t S_descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* S_cmdList;

	static uint64_t S_GpuStartHandle;
	// �S�ẴI�u�W�F�N�g���������z��
	static std::list<std::unique_ptr<ADXObject, ADXUtility::NPManager<ADXObject>>> S_objs;
	// �S�ẴJ����������z��
	static std::vector<ADXCamera*> S_allCameraPtr;
	// �I�u�W�F�N�g�����݂ł���̈�𐧌����邽�߂̕ϐ�
	static ADXVector3 S_limitPos1;
	static ADXVector3 S_limitPos2;

	static bool S_highQualityZSort;

	static std::list<std::unique_ptr<ADXComponent, ADXUtility::NPManager<ADXComponent>>> S_usedComponents;

public:
	void Draw();
	void Destroy();
	template <class Type>
	Type* AddComponent();
	template <class Type>
	Type* GetComponent();
	template <class Type>
	std::list<Type*> GetComponents();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
	bool GetDeleteFlag() { return deleteFlag_; };
	ID3D12Resource* GetConstBuffB1() { return constBuffB1_.Get(); };

private:
	void Initialize();
	void Update();
	void CreateConstBuffer();

public: // �ÓI�����o�֐�
	//�ÓI������
	static void StaticInitialize();
	//�ÓI�X�V����
	static void StaticUpdate();
	// �O���t�B�b�N�p�C�v���C������
	static void InitializeGraphicsPipeline();
	// �g�����X�t�H�[���p�萔�o�b�t�@����
	static void InitializeConstBufferTransform(Microsoft::WRL::ComPtr<ID3D12Resource>& constBuff, ConstBufferDataTransform** constMap);
	// �}�e���A���p�萔�o�b�t�@����
	static void InitializeConstBufferMaterial(Microsoft::WRL::ComPtr<ID3D12Resource>& constBuff);
	// �S�I�u�W�F�N�g�ɑ΂���`�揈��
	static void StaticDraw();
	// �R�}���h���X�g���擾
	static ID3D12GraphicsCommandList* GetCmdList() { return S_cmdList; };

	static uint64_t GetGpuStartHandle() { return S_GpuStartHandle; };
	// �S�I�u�W�F�N�g���擾
	static std::list<ADXObject*> GetObjs();

	static void SetAllCameraPtr(ADXCamera* camPtr);

	//��̃I�u�W�F�N�g�𐶐�
	static ADXObject* Create(const ADXVector3& setLocalPosition = { 0,0,0 },
		const ADXQuaternion& setLocalRotation = ADXQuaternion::IdentityQuaternion(),
		const ADXVector3& setLocalScale = { 1,1,1 }, ADXWorldTransform* setParent = nullptr);

	//�����̃I�u�W�F�N�g�𕡐�
	static ADXObject* Duplicate(const ADXObject& prefab);

	//�V�[���؂�ւ����ȂǂɎg�p�A������I�u�W�F�N�g��S������
	static void Annihilate() { S_objs.clear(); };

private:
	// �`��O����
	static void PreDraw();
	// �`��㏈��
	static void PostDraw();
};

template<class Type>
inline Type* ADXObject::AddComponent()
{
	Type* p = new Type();
	std::unique_ptr<ADXComponent, ADXUtility::NPManager<ADXComponent>> temp(p);
	temp->SetGameObject(this);
	components_.push_back(move(temp));
	return p;
}

template<class Type>
inline Type* ADXObject::GetComponent()
{
	if (this == nullptr)
	{
		return nullptr;
	}

	for (auto& itr : components_)
	{
		if (itr)
		{
			Type* p = dynamic_cast<Type*>(itr.get());

			if (p != nullptr)
			{
				return p;
			}
		}
	}
	return nullptr;
}

template<class Type>
inline std::list<Type*> ADXObject::GetComponents()
{
	if (this == nullptr)
	{
		return std::list<Type*>{};
	}

	std::list<Type*> ret = {};
	for (auto& itr : components_)
	{
		if (itr)
		{
			Type* p = dynamic_cast<Type*>(itr.get());

			if (p != nullptr)
			{
				ret.push_back(p);
			}
		}
	}
	return ret;
}