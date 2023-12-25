#include "ADXModel.h"
#include "ADXCommon.h"
#include "ADXObject.h"
#include <math.h>

#include <fstream>
#include <sstream>

using namespace DirectX;

ADXModel::ADXModel()
{

}

ADXModel ADXModel::LoadADXModel(const std::string& filePath)
{
	ADXModel model;

	std::ifstream file;
	file.open("Resources/" + filePath);

	if (file.fail())
	{
		assert(0);
	}

	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream line_stream(line);
		std::string key_;
		std::getline(line_stream, key_, ' ');

		//�擪������v�Ȃ璸�_���W
		if (key_ == "v")
		{
			//XYZ���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}

		//�擪������vt�Ȃ�e�N�X�`��
		if (key_ == "vt")
		{
			//UV�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		//�擪������vn�Ȃ�@���x�N�g��
		if (key_ == "vn")
		{
			//XYZ�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		//�擪������f�Ȃ�|���S��
		if (key_ == "f")
		{
			int32_t polyCount = 0;
			unsigned short firstIndex{};
			unsigned short lastIndex{};
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			while (std::getline(line_stream, index_string, ' '))
			{
				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;
				//���_�f�[�^�̒ǉ�
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				model.vertices_.emplace_back(vertex);
				//�C���f�b�N�X�f�[�^�̒ǉ�
				unsigned short nowIndex = (unsigned short)model.vertices_.size() - 1;
				model.indices_.emplace_back(nowIndex);
				if (polyCount >= 3)
				{
					model.indices_.emplace_back(firstIndex);
					model.indices_.emplace_back(lastIndex);
				}
				if (polyCount == 0)
				{
					firstIndex = nowIndex;
				}
				lastIndex = nowIndex;
				polyCount++;
			}
		}
	}
	file.close();

	model.Initialize();

	return model;
}

void ADXModel::SetNormal()
{
	for (int32_t i = 0; i < indices_.size() / 3; i++)
	{//�O�p�`����ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices_[i * 3];
		unsigned short index1 = indices_[i * 3 + 1];
		unsigned short index2 = indices_[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices_[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices_[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices_[index2].pos);
		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices_[index0].normal, normal);
		XMStoreFloat3(&vertices_[index1].normal, normal);
		XMStoreFloat3(&vertices_[index2].normal, normal);
	}
}

///<summary>
///���_�o�b�t�@�r���[���쐬���鏈�����ꊇ�ōs��
///</summary>
void ADXModel::CreateVertexBufferView()
{
	HRESULT result;

	//���_�f�[�^�S�̂̃T�C�Y = ��̒��_�f�[�^�̃T�C�Y * ���_�f�[�^�̗v�f��
	uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());
	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//���_�o�b�t�@�̐���
	result = ADXCommon::GetCurrentInstance()->GetDevice()
		->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	//GPU��̃o�b�t�@�ɑΉ��������z�������i���C����������j���擾
	Vertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂����W���R�s�[
	for (int32_t i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i];
	}
	//�Ȃ�����폜
	vertBuff_->Unmap(0, nullptr);

	//GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB;
	//���_����̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);
}

///<summary>
///�C���f�b�N�X�o�b�t�@�r���[���쐬���鏈�����ꊇ�ōs��
///</summary>
void ADXModel::CreateIndexBufferView()
{
	HRESULT result;

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�C���f�b�N�X�o�b�t�@�̐���
	result = ADXCommon::GetCurrentInstance()->GetDevice()
		->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff_));

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂��C���f�b�N�X���R�s�[
	for (int32_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];
	}
	//�Ȃ�����폜
	indexBuff_->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

void ADXModel::Initialize()
{
	//�@���������ݒ�
	SetNormal();

	//���_�o�b�t�@�r���[�쐬
	CreateVertexBufferView();

	//�C���f�b�N�X�o�b�t�@�r���[�쐬
	CreateIndexBufferView();
}

void ADXModel::Draw(ID3D12Resource* constBuffTransform)
{
	ID3D12GraphicsCommandList* commandList = ADXObject::GetCmdList();

	// ���f���f�[�^���X�V
	Update();

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	commandList->IASetVertexBuffers(0, 1, &vbView_);

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	commandList->IASetIndexBuffer(&ibView_);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	// �`��R�}���h
	commandList->DrawIndexedInstanced((uint32_t)indices_.size(), 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
}

void ADXModel::Update()
{
	//�@���������ݒ�
	SetNormal();

	HRESULT result;

	//���_�f�[�^�S�̂̃T�C�Y = ��̒��_�f�[�^�̃T�C�Y * ���_�f�[�^�̗v�f��
	uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());

	//GPU��̃o�b�t�@�ɑΉ��������z�������i���C����������j���擾
	Vertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂����W���R�s�[
	for (int32_t i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i];
	}
	//�Ȃ�����폜
	vertBuff_->Unmap(0, nullptr);

	//GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB;
	//���_����̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);


	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂��C���f�b�N�X���R�s�[
	for (int32_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];
	}
	//�Ȃ�����폜
	indexBuff_->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

ADXModel ADXModel::CreateRect()
{
	ADXModel rect_;
	rect_.vertices_ = {
	{{-1.0f,-1.0f,0.0f},{}, {0.0f,1.0f}},//����
	{{-1.0f,1.0f,0.0f},{},{0.0f,0.0f}},//����
	{{1.0f,-1.0f,0.0f},{},{1.0f,1.0f}},//�E��
	{{1.0f,1.0f,0.0f},{},{1.0f,0.0f}},//�E��
	};
	//�C���f�b�N�X�f�[�^
	rect_.indices_ =
	{
		0,1,2,
		2,1,3,

		1,0,2,
		1,2,3,
	};
	rect_.Initialize();
	return rect_;
}