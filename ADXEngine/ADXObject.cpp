#include "ADXObject.h"
#include "ADXCommon.h"
#include "ADXSceneManager.h"
#include "ADXCamera.h"
#include "ADXRenderer.h"
#include <d3dcompiler.h>
#include <imgui.h>

#pragma comment(lib, "d3dcompiler.lib")

uint64_t ADXObject::S_descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ADXObject::S_cmdList = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> ADXObject::S_rootSignature;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ADXObject::S_pipelineState;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ADXObject::S_pipelineStateAlpha;

uint64_t ADXObject::S_GpuStartHandle = 0;
std::list<std::unique_ptr<ADXObject, ADXUtility::NPManager<ADXObject>>> ADXObject::S_objs{};
std::vector<ADXCamera*> ADXObject::S_allCameraPtr{};
ADXVector3 ADXObject::S_limitPos1 = { -300,-300,-100 };
ADXVector3 ADXObject::S_limitPos2 = { 100,100,150 };
bool ADXObject::S_highQualityZSort = false;
std::list<std::unique_ptr<ADXComponent, ADXUtility::NPManager<ADXComponent>>> ADXObject::S_usedComponents;



void ADXObject::StaticInitialize()
{
	// �p�C�v���C��������
	InitializeGraphicsPipeline();
}

void ADXObject::Initialize()
{
	*this = {};
	CreateConstBuffer();
	transform_.Initialize(this);
}

void ADXObject::CreateConstBuffer()
{
	InitializeConstBufferMaterial(constBuffB1_);
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

void ADXObject::InitializeConstBufferTransform(Microsoft::WRL::ComPtr<ID3D12Resource>& constBuffTransform, ConstBufferDataTransform** constMapTransform)
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
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));

		//�萔�o�b�t�@�̃}�b�s���O
		result = constBuffTransform->Map(0, nullptr, (void**)constMapTransform);//�}�b�s���O
		assert(SUCCEEDED(result));
	}
}

void ADXObject::InitializeConstBufferMaterial(Microsoft::WRL::ComPtr<ID3D12Resource>& constBuff)
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
			IID_PPV_ARGS(&constBuff));
		assert(SUCCEEDED(result));
	}
}

void ADXObject::SetAllCameraPtr(ADXCamera* camPtr)
{
	S_allCameraPtr.push_back(camPtr);
}

ADXObject* ADXObject::Create(const ADXVector3& setLocalPosition, const ADXQuaternion& setLocalRotation,
	const ADXVector3& setLocalScale, ADXWorldTransform* setParent)
{
	std::unique_ptr<ADXObject, ADXUtility::NPManager<ADXObject>> obj(new ADXObject);
	obj->Initialize();
	obj->transform_.localPosition_ = setLocalPosition;
	obj->transform_.localRotation_ = setLocalRotation;
	obj->transform_.localScale_ = setLocalScale;
	obj->transform_.parent_ = setParent;
	S_objs.push_back(move(obj));
	return S_objs.back().get();
}

ADXObject* ADXObject::Duplicate(const ADXObject& prefab)
{
	ADXObject* ret = Create(prefab.transform_.localPosition_, prefab.transform_.localRotation_,
		prefab.transform_.localScale_, prefab.transform_.parent_);
	ret->transform_.rectTransform_ = prefab.transform_.rectTransform_;
	ret->transform_.modelPosition_ = prefab.transform_.modelPosition_;
	ret->transform_.modelRotation_ = prefab.transform_.modelRotation_;
	ret->transform_.modelScale_ = prefab.transform_.modelScale_;
	ret->model_ = prefab.model_;
	ret->material_ = prefab.material_;
	ret->texture_ = prefab.texture_;
	ret->renderLayer_ = prefab.renderLayer_;
	ret->sortingOrder_ = prefab.sortingOrder_;
	ret->alphaTex_ = prefab.alphaTex_;
	ret->isVisible_ = prefab.isVisible_;
	ret->isActive_ = prefab.isActive_;
	ret->useDefaultDraw_ = prefab.useDefaultDraw_;

	return ret;
}

void ADXObject::Update()
{
	if (isActive_)
	{
		for (auto& itr : components_)
		{
			itr->Update(this);
		}
	}
}

void ADXObject::StaticUpdate()
{
	ADXVector3 limitMinPos = { min(S_limitPos1.x_,S_limitPos2.x_),min(S_limitPos1.y_,S_limitPos2.y_) ,min(S_limitPos1.z_,S_limitPos2.z_) };
	ADXVector3 limitMaxPos = { max(S_limitPos1.x_,S_limitPos2.x_),max(S_limitPos1.y_,S_limitPos2.y_) ,max(S_limitPos1.z_,S_limitPos2.z_) };

	for (auto& itr : S_objs)
	{
		for (auto& comItr : itr->components_)
		{
			comItr->SafetyPhase();
		}
	}

	for (auto& itr : S_objs)
	{
		if (itr->deleteFlag_ || (itr->transform_.parent_ != nullptr && itr->transform_.parent_->GetGameObject()->deleteFlag_))
		{
			itr->components_.clear();
		}
	}
	S_objs.remove_if([=](auto& itr)
		{ return itr->deleteFlag_; });

	for (auto& itr : S_objs)
	{
		#ifdef _DEBUG
		float pos[3] = { itr->transform_.localPosition_.x_,itr->transform_.localPosition_.y_,itr->transform_.localPosition_.z_ };

		bool tool_active = true;
		ImGui::Begin("My First Tool", &tool_active, ImGuiWindowFlags_MenuBar);
		ImGui::InputFloat3("Position", pos);

		ImGui::End();

		itr->transform_.localPosition_ = { pos[0],pos[1],pos[2] };
		#endif

		itr->Update();
		if (itr->transform_.parent_ == nullptr && !itr->transform_.rectTransform_)
		{
			ADXVector3 itrWorldPos = itr->transform_.GetWorldPosition();
			itr->transform_.SetWorldPosition({
				max(limitMinPos.x_, min(itrWorldPos.x_, limitMaxPos.x_)),
				max(limitMinPos.y_, min(itrWorldPos.y_, limitMaxPos.y_)),
				max(limitMinPos.z_, min(itrWorldPos.z_, limitMaxPos.z_))
				});
		}
	}
}

void ADXObject::StaticDraw()
{
	PreDraw();

	std::list<ADXObject*> allObjPtr = GetObjs();

	for (int32_t i = 0; i < S_allCameraPtr.size(); i++)
	{
		S_allCameraPtr[i]->PrepareToRandering();
	}

	for (auto& itr : allObjPtr)
	{
		for (auto& comItr : itr->components_)
		{
			comItr->OnPreRender();
		}
	}

	int32_t minLayer = 0;
	int32_t maxLayer = 0;
	int32_t minSortingOrder = 0;
	int32_t maxSortingOrder = 0;

	if (allObjPtr.size() > 0)
	{
		minLayer = allObjPtr.front()->renderLayer_;
		maxLayer = allObjPtr.front()->renderLayer_;
		minSortingOrder = allObjPtr.front()->sortingOrder_;
		maxSortingOrder = allObjPtr.front()->sortingOrder_;
	}

	//RenderLayer�͈̔͂�ǂݎ��
	for (auto& itr : allObjPtr)
	{
		if (itr->renderLayer_ < minLayer)
		{
			minLayer = itr->renderLayer_;
		}
		if (itr->renderLayer_ > maxLayer)
		{
			maxLayer = itr->renderLayer_;
		}

		if (itr->sortingOrder_ < minSortingOrder)
		{
			minSortingOrder = itr->sortingOrder_;
		}
		if (itr->sortingOrder_ > maxSortingOrder)
		{
			maxSortingOrder = itr->sortingOrder_;
		}
	}

	for (int32_t nowLayer = minLayer; nowLayer <= maxLayer; nowLayer++)
	{
		std::vector<ADXObject*> thisLayerObjPtr;

		// �p�C�v���C���X�e�[�g�̐ݒ�R�}���h
		S_cmdList->SetPipelineState(S_pipelineState.Get());
		//�S�s�N�Z���̐[�x�o�b�t�@�l���ŉ���1.0�ɂ���
		S_cmdList->ClearDepthStencilView(*ADXCommon::GetCurrentInstance()->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		for (auto& itr : allObjPtr)
		{
			if (itr->renderLayer_ == nowLayer)
			{
				thisLayerObjPtr.push_back(itr);
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
				thisLayerObjPtr[j]->transform_.UpdateMatrix();
				float zDepth;

				if (S_highQualityZSort)
				{
					float nearestVertDepth = 999;

					if (thisLayerObjPtr[j]->model_ != nullptr)
					{
						for (int32_t k = 0; k < thisLayerObjPtr[j]->model_->vertices_.size(); k++)
						{
							float VertDepth;
							ADXVector3 vertLocalPos = { thisLayerObjPtr[j]->model_->vertices_[k].pos.x,thisLayerObjPtr[j]->model_->vertices_[k].pos.y,thisLayerObjPtr[j]->model_->vertices_[k].pos.z };
							VertDepth = ADXMatrix4::Transform(ADXMatrix4::Transform(vertLocalPos, thisLayerObjPtr[j]->transform_.GetMatWorld()), ADXWorldTransform::GetViewProjection()).Length();

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
					zDepth = ADXMatrix4::Transform(thisLayerObjPtr[j]->transform_.GetWorldPosition(), ADXWorldTransform::GetViewProjection()).Length();
				}

				if (thisLayerObjPtr[j]->sortingOrder_ < lowestSortingOrder || thisLayerObjPtr[j]->sortingOrder_ == lowestSortingOrder && zDepth >= dist)
				{
					dist = zDepth;
					target = j;
					lowestSortingOrder = thisLayerObjPtr[j]->sortingOrder_;
				}
			}
			if (thisLayerObjPtr[target]->isVisible_ && thisLayerObjPtr[target]->isActive_)
			{
				thisLayerObjPtr[target]->Draw();
			}
			thisLayerObjPtr.erase(thisLayerObjPtr.begin() + target);
		}
	}
	PostDraw();
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

void ADXObject::PostDraw()
{
	// �R�}���h���X�g������
	ADXObject::S_cmdList = nullptr;
	//�\�[�g�p�z�����ɂ���
	S_allCameraPtr.clear();
}

std::list<ADXObject*> ADXObject::GetObjs()
{
	std::list<ADXObject*> ret = {};

	for (auto& itr : S_objs)
	{
		ret.push_back(itr.get());
	}

	return ret;
}

void ADXObject::Draw()
{
	for (auto& itr : components_)
	{
		itr->OnWillRenderObject();
	}

	// nullptr�`�F�b�N
	[[maybe_unused]]ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();
	assert(device);
	assert(S_cmdList);

	if (useDefaultDraw_ && model_ != nullptr)
	{
		HRESULT result = S_FALSE;
		//�萔�o�b�t�@�փf�[�^�]��
		ConstBufferDataB1* constMap1 = nullptr;
		result = constBuffB1_->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material_.ambient_;
		constMap1->diffuse = material_.diffuse_;
		constMap1->specular = material_.specular_;
		constMap1->alpha = material_.alpha_;
		constBuffB1_->Unmap(0, nullptr);

		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		S_cmdList->SetGraphicsRootConstantBufferView(2, constBuffB1_->GetGPUVirtualAddress());

		D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;
		S_gpuDescHandleSRV.ptr = S_GpuStartHandle + texture_;
		S_cmdList->SetGraphicsRootDescriptorTable(1, S_gpuDescHandleSRV);
		
		transform_.UpdateConstBuffer();

		// �`��R�}���h
		model_->Draw(transform_.constBuffTransform_.Get());
	}

	for (auto& itr : GetComponents<ADXRenderer>())
	{
		itr->Rendering();
	}

	for (auto& itr : components_)
	{
		itr->Rendered();
	}
}

void ADXObject::Destroy()
{
	deleteFlag_ = true;
	for (auto& itr : components_)
	{
		itr->OnDestroy();
	}
	for (auto& itr : transform_.GetChilds())
	{
		itr->GetGameObject()->Destroy();
	}
}

void ADXObject::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	for (auto& itr : components_)
	{
		itr->OnCollisionHit(col, myCol);
	}
}