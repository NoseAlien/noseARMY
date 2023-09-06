#pragma once
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXImage.h"
#include "ADXCollider.h"
#include <string>

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
	ADXObject();
	void Initialize();
	void CreateConstBuffer();
	void Update();
	void Draw();
	void Destroy();
	void InitCols();
	virtual void OnCollisionHit(ADXCollider* col, ADXCollider* myCol) {};

protected:
	virtual void UniqueUpdate() {};
	virtual void OnPreRender() {};
	virtual void OnWillRenderObject() {};
	virtual void Rendered() {};

public:
	ADXWorldTransform transform{};
	ADXModel* model = nullptr;
	ADXMaterial material{};
	uint32_t texture = 0;
	std::vector<ADXCollider> colliders{};
	int32_t renderLayer = 0;
	int32_t sortingOrder = 0;
	bool alphaTex = false;
	bool isVisible = true;
	bool isActive = true;
	bool useDefaultDraw = true;
	float maxMoveDistanceRate = 4;

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1 = nullptr;
	ConstBufferDataB0* constMapMaterial = nullptr;

private:
	bool deleteFlag = false;

public: // �ÓI�����o�֐�
	//�ÓI������
	static void StaticInitialize();
	//�ÓI�X�V����
	static void StaticUpdate();
	// �O���t�B�b�N�p�C�v���C������
	static void InitializeGraphicsPipeline();
	// �`��O����
	static void PreDraw();
	// �`�揈��
	static void StaticDraw();
	// �`��㏈��
	static void PostDraw();

	static std::vector<ADXObject*> GetObjs() { return S_objs; };

	static void SetAllCameraPtr(ADXCamera* camPtr);

	static ADXObject Duplicate(const ADXObject& prefab, bool initCols = false);

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
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static D3D12_CPU_DESCRIPTOR_HANDLE S_cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;

	static uint64_t S_GpuStartHandle;
	// �S�ẴI�u�W�F�N�g������z��
	static std::vector<ADXObject*> S_allObjPtr;
	// �S�ẴI�u�W�F�N�g���������z��
	static std::vector<ADXObject*> S_objs;
	// �S�ẴJ����������z��
	static std::vector<ADXCamera*> S_allCameraPtr;
	// �I�u�W�F�N�g�����݂ł���̈�𐧌����邽�߂̕ϐ�
	static ADXVector3 S_limitPos1;
	static ADXVector3 S_limitPos2;
	
	static bool S_highQualityZSort;
};