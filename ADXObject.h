#pragma once
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXImage.h"
#include "ADXComponentInclude.h"
#include <string>

#pragma comment(lib, "d3dcompiler.lib")

class ADXObject
{
public:

	//�萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j

	struct ConstBufferDataB0 {
		XMMATRIX mat;
	};

	struct ConstBufferDataB1 {
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

public:
	ADXObject();
	void Initialize();
	void CreateConstBuffer();
	void Update();
	void Draw(UINT64 GpuStartHandle);

protected:
	virtual void UniqueUpdate();

public:
	ADXWorldTransform transform{};
	ADXModel* model = nullptr;
	ADXMaterial material{};
	ADXImage texture{};
	std::vector<ADXCollider> colliders{};
	int renderLayer = 0;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1 = nullptr;

	ConstBufferDataB0* constMapMaterial = nullptr;

public: // �ÓI�����o�֐�
	//�ÓI������
	static void StaticInitialize(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);

	//�ÓI�X�V����
	static void StaticUpdate();

	// �O���t�B�b�N�p�C�v���C������
	static void InitializeGraphicsPipeline();

	// �`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	// �`�揈��
	static void StaticDraw(ID3D12DescriptorHeap* srvHeap);

	// �`��㏈��
	static void PostDraw();

	static std::vector<ADXObject*> GetAllObjs();

	static ADXObject Duplicate(ADXObject prefab);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// �f�X�N���v�^�q�[�v
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	static Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	static Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	static D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle;
	// �r���[�s��
	static ADXMatrix4 matView;
	// �ˉe�s��
	static ADXMatrix4 matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// �S�ẴI�u�W�F�N�g������z��
	static std::vector<ADXObject*> allObjPtr;
	// �S�ẴI�u�W�F�N�g���������z��
	static std::vector<ADXObject*> objs;
};