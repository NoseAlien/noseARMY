#include "ADXObject.h"
#include <d3dcompiler.h>
#include "ADXCommon.h"
#include "ADXSceneManager.h"
#include "ADXCamera.h"

#pragma comment(lib, "d3dcompiler.lib")

uint64_t ADXObject::S_descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ADXObject::S_cmdList = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> ADXObject::S_rootSignature;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ADXObject::S_pipelineState;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ADXObject::S_pipelineStateAlpha;

uint64_t ADXObject::S_GpuStartHandle = 0;
D3D12_CPU_DESCRIPTOR_HANDLE ADXObject::S_cpuDescHandleSRV;
D3D12_GPU_DESCRIPTOR_HANDLE ADXObject::S_gpuDescHandleSRV;
std::vector<ADXObject*> ADXObject::S_allObjPtr{};
std::vector<ADXObject*> ADXObject::S_objs{};
std::vector<ADXCamera*> ADXObject::S_allCameraPtr{};
ADXVector3 ADXObject::S_limitPos1 = { -300,-300,-100 };
ADXVector3 ADXObject::S_limitPos2 = { 100,100,150 };
bool ADXObject::S_highQualityZSort = false;

ADXObject::ADXObject()
{

}

void ADXObject::StaticInitialize()
{
	// �p�C�v���C��������
	InitializeGraphicsPipeline();
}

void ADXObject::Initialize()
{
	*this = {};
	transform.Initialize();
	CreateConstBuffer();
}

void ADXObject::CreateConstBuffer()
{
	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();

	if (device != nullptr)
	{
		HRESULT result = S_FALSE;

		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataB0) + 0xff) & ~0xff;//256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//�萔�o�b�t�@�̐���
		result = device->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&transform.constBuffTransform));
		assert(SUCCEEDED(result));
		//�萔�o�b�t�@�̃}�b�s���O
		result = transform.constBuffTransform->Map(0, nullptr, (void**)&transform.constMapTransform);//�}�b�s���O
		assert(SUCCEEDED(result));


		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataB1) + 0xff) & ~0xff;//256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//�萔�o�b�t�@�̐���
		result = device->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffB1));
		assert(SUCCEEDED(result));
	}
}

void ADXObject::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shader/OBJVertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);

	//��̓ǂݍ��݂ŃG���[���N������o�̓E�B���h�E�ɓ��e��\��
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";

		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shader/OBJPixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	//��̓ǂݍ��݂ŃG���[���N������o�̓E�B���h�E�ɓ��e��\��
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";

		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//�O�������W
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//�@���x�N�g��
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//uv���W
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; //�w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineDesc.RasterizerState.DepthClipEnable = true;

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true; //�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; //�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //��������������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineDesc.BlendState.AlphaToCoverageEnable = true;

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//�u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��ݒ�i�O�p�`�j
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc.SampleDesc.Count = 1;

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;//��x�̕`��Ɏg���摜��1���Ȃ̂�
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�[�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//���
	rootParams[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����
	//�e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����
	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//���
	rootParams[2].Descriptor.ShaderRegister = 1;//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�(�^�C�����O)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ�(�^�C�����O)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ�(�^�C�����O)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;//�S�ă��j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_�[����̂ݎg�p�\

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;//���[�g�p�����[�^�[�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);//���[�g�p�����[�^�[��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();

	//���[�g�V�O�l�`���̃V���A���C�Y
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	//���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&S_rootSignature));
	assert(SUCCEEDED(result));
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = S_rootSignature.Get();
	// �p�C�v���C���X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&S_pipelineState));
	assert(SUCCEEDED(result));

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //�������ݕs��

	// �p�C�v���C���X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&S_pipelineStateAlpha));
	assert(SUCCEEDED(result));
}

void ADXObject::SetAllCameraPtr(ADXCamera* camPtr)
{
	S_allCameraPtr.push_back(camPtr);
}

ADXObject ADXObject::Duplicate(const ADXObject& prefab, bool initCols)
{
	ADXObject ret = prefab;
	ret.CreateConstBuffer();
	if (initCols)
	{
		ret.InitCols();
	}
	return ret;
}

void ADXObject::Update()
{
	for (int32_t i = 0; i < colliders.size(); i++)
	{
		colliders[i].Update(this);
	}
	UniqueUpdate();

	S_allObjPtr.push_back(this);
}

void ADXObject::UniqueUpdate()
{

}

void ADXObject::OnPreRender()
{
}

void ADXObject::StaticUpdate()
{
	S_objs = S_allObjPtr;

	ADXVector3 limitMinPos = { min(S_limitPos1.x,S_limitPos2.x),min(S_limitPos1.y,S_limitPos2.y) ,min(S_limitPos1.z,S_limitPos2.z) };
	ADXVector3 limitMaxPos = { max(S_limitPos1.x,S_limitPos2.x),max(S_limitPos1.y,S_limitPos2.y) ,max(S_limitPos1.z,S_limitPos2.z) };

	for (auto& itr : GetObjs())
	{
		if (itr->transform.parent_ == nullptr && !itr->transform.rectTransform)
		{
			ADXVector3 itrWorldPos = itr->transform.GetWorldPosition();
			itr->transform.SetWorldPosition({
				max(limitMinPos.x, min(itrWorldPos.x, limitMaxPos.x)),
				max(limitMinPos.y, min(itrWorldPos.y, limitMaxPos.y)),
				max(limitMinPos.z, min(itrWorldPos.z, limitMaxPos.z))
				});
		}
	}
}

void ADXObject::PreDraw()
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(ADXObject::S_cmdList == nullptr);

	// �R�}���h���X�g���Z�b�g
	ADXObject::S_cmdList = ADXCommon::GetCurrentInstance()->GetCommandList();

	// �p�C�v���C���X�e�[�g�̐ݒ�
	ADXObject::S_cmdList->SetPipelineState(S_pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	ADXObject::S_cmdList->SetGraphicsRootSignature(S_rootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	ADXObject::S_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g


	ID3D12DescriptorHeap* srvHeap = ADXImage::GetSrvHeap();

	//SRV�q�[�v�̐ݒ�R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap };
	ADXObject::S_cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^�[1�Ԃɐݒ�
	ADXObject::S_cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	S_GpuStartHandle = srvGpuHandle.ptr;

	// ���[�g�V�O�l�`���̐ݒ�R�}���h
	ADXObject::S_cmdList->SetGraphicsRootSignature(S_rootSignature.Get());
}

void ADXObject::StaticDraw()
{
	for (int32_t i = 0; i < S_allCameraPtr.size(); i++)
	{
		S_allCameraPtr[i]->PrepareToRandering();
	}

	for (int32_t i = 0; i < S_allObjPtr.size(); i++)
	{
		S_allObjPtr[i]->OnPreRender();
	}

	int32_t minLayer = 0;
	int32_t maxLayer = 0;
	int32_t minSortingOrder = 0;
	int32_t maxSortingOrder = 0;

	if (S_allObjPtr.size() > 0)
	{
		minLayer = S_allObjPtr[0]->renderLayer;
		maxLayer = S_allObjPtr[0]->renderLayer;
		minSortingOrder = S_allObjPtr[0]->sortingOrder;
		maxSortingOrder = S_allObjPtr[0]->sortingOrder;
	}

	//RenderLayer�͈̔͂�ǂݎ��
	for (int32_t i = 0; i < S_allObjPtr.size(); i++)
	{
		if (S_allObjPtr[i]->renderLayer < minLayer)
		{
			minLayer = S_allObjPtr[i]->renderLayer;
		}
		if (S_allObjPtr[i]->renderLayer > maxLayer)
		{
			maxLayer = S_allObjPtr[i]->renderLayer;
		}

		if (S_allObjPtr[i]->sortingOrder < minSortingOrder)
		{
			minSortingOrder = S_allObjPtr[i]->sortingOrder;
		}
		if (S_allObjPtr[i]->sortingOrder > maxSortingOrder)
		{
			maxSortingOrder = S_allObjPtr[i]->sortingOrder;
		}
	}

	for (int32_t nowLayer = minLayer; nowLayer <= maxLayer; nowLayer++)
	{
		std::vector<ADXObject*> thisLayerObjPtr;

		// �p�C�v���C���X�e�[�g�̐ݒ�R�}���h
		S_cmdList->SetPipelineState(S_pipelineState.Get());
		//�S�s�N�Z���̐[�x�o�b�t�@�l���ŉ���1.0�ɂ���
		S_cmdList->ClearDepthStencilView(*ADXCommon::GetCurrentInstance()->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		for (int32_t i = 0; i < S_allObjPtr.size(); i++)
		{
			if (S_allObjPtr[i]->renderLayer == nowLayer)
			{
				thisLayerObjPtr.push_back(S_allObjPtr[i]);
			}
		}

		//���߂��I�u�W�F�N�g��[�x�̐[�����Ɏ��o���Ȃ���`��
		for (int32_t i = 0; thisLayerObjPtr.size() > 0; i++)
		{
			int32_t lowestSortingOrder = minSortingOrder;
			float dist = 0;
			int32_t target = 0;
			for (int32_t j = 0; j < thisLayerObjPtr.size(); j++)
			{
				thisLayerObjPtr[j]->transform.UpdateMatrix();
				float zDepth;

				if (S_highQualityZSort)
				{
					float nearestVertDepth = 999;

					if (thisLayerObjPtr[j]->model != nullptr)
					{
						for (int32_t k = 0; k < thisLayerObjPtr[j]->model->vertices.size(); k++)
						{
							float VertDepth;
							ADXVector3 vertLocalPos = { thisLayerObjPtr[j]->model->vertices[k].pos.x,thisLayerObjPtr[j]->model->vertices[k].pos.y,thisLayerObjPtr[j]->model->vertices[k].pos.z };
							VertDepth = ADXMatrix4::transform(ADXMatrix4::transform(vertLocalPos, thisLayerObjPtr[j]->transform.GetMatWorld()), ADXWorldTransform::GetViewProjection()).Length();

							if (VertDepth <= nearestVertDepth)
							{
								nearestVertDepth = VertDepth;
							}
						}
					}

					zDepth = nearestVertDepth;
				}
				else
				{
					zDepth = ADXMatrix4::transform(thisLayerObjPtr[j]->transform.GetWorldPosition(), ADXWorldTransform::GetViewProjection()).Length();
				}

				if (thisLayerObjPtr[j]->sortingOrder < lowestSortingOrder || thisLayerObjPtr[j]->sortingOrder == lowestSortingOrder && zDepth >= dist)
				{
					dist = zDepth;
					target = j;
					lowestSortingOrder = thisLayerObjPtr[j]->sortingOrder;
				}
			}
			if (thisLayerObjPtr[target]->isVisible)
			{
				thisLayerObjPtr[target]->Draw();
			}
			thisLayerObjPtr.erase(thisLayerObjPtr.begin() + target);
		}
	}
}

void ADXObject::PostDraw()
{
	// �R�}���h���X�g������
	ADXObject::S_cmdList = nullptr;
	//�\�[�g�p�z�����ɂ���
	S_allObjPtr.clear();
	S_allCameraPtr.clear();
}

void ADXObject::Draw()
{
	OnWillRenderObject();

	// nullptr�`�F�b�N
	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();
	assert(device);
	assert(S_cmdList);

	if (useDefaultDraw && model != nullptr)
	{
		HRESULT result = S_FALSE;
		//�萔�o�b�t�@�փf�[�^�]��
		ConstBufferDataB1* constMap1 = nullptr;
		result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		constBuffB1->Unmap(0, nullptr);

		S_gpuDescHandleSRV.ptr = S_GpuStartHandle + texture;
		S_cmdList->SetGraphicsRootDescriptorTable(1, S_gpuDescHandleSRV);

		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		S_cmdList->SetGraphicsRootConstantBufferView(2, constBuffB1->GetGPUVirtualAddress());
		
		transform.UpdateConstBuffer();

		// �`��R�}���h
		model->Draw(S_cmdList, transform);
	}

	Rendered();
}

void ADXObject::Destroy()
{
	deleteFlag = true;
}

void ADXObject::OnWillRenderObject()
{

}

void ADXObject::Rendered()
{

}

void ADXObject::InitCols()
{
	for (int32_t i = 0; i < colliders.size(); i++)
	{
		colliders[i].Initialize(this);
	}
}

void ADXObject::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
}
