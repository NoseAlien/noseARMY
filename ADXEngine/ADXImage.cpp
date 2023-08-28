#include "ADXimage.h"
#include "ADXDataPool.h"
#include "ADXCommon.h"
#include "ADXUtility.h"

using namespace DirectX;

D3D12_CPU_DESCRIPTOR_HANDLE ADXImage::S_srvHandle = {};
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ADXImage::S_srvHeap = nullptr;
uint64_t ADXImage::S_CpuStartHandle = 0;
uint64_t ADXImage::S_incrementSize = 0;

ADXImage::ADXImage()
{

}

void ADXImage::StaticInitialize()
{
	HRESULT result;

	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();

	//�V�F�[�_�[���\�[�X�r���[�̍ő��
	const uint32_t kMaxSRVCount = 2056;
	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;
	//�ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&S_srvHeap));
	assert(SUCCEEDED(result));

	//SRV�q�[�v�̐擪�n���h�����擾
	S_srvHandle = S_srvHeap->GetCPUDescriptorHandleForHeapStart();
	S_CpuStartHandle = S_srvHandle.ptr;

	//�f�X�N���v�^�̃T�C�Y
	//�h���C�o�ɂ���ĈႤ�̂Ŋ֐��Ŏ擾���Ȃ���΂Ȃ�Ȃ�
	S_incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

uint32_t ADXImage::LoadADXImage(const std::string& imgName)
{
	HRESULT result;
	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();

	ADXImage image;

	std::vector<ADXImage> imgDataPool = ADXDataPool::GetImgDataPool();

	for (int32_t i = 0; i < imgDataPool.size(); i++)
	{
		if (imgDataPool[i].name == imgName)
		{
			image.Ghandle = imgDataPool[i].Ghandle;
			return image.Ghandle;
		}
	}

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	D3D12_RESOURCE_DESC textureResourceDesc{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����

	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		ADXUtility::StringToWideChar("Resources/" + imgName),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	//�q�[�v�ݒ�
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (uint32_t)metadata.height;
	textureResourceDesc.DepthOrArraySize = (uint16_t)metadata.arraySize;
	textureResourceDesc.MipLevels = (uint16_t)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&image.texBuff));
	//�S�~�b�v�}�b�v�ɂ���
	for (uint32_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = image.texBuff->WriteToSubresource(
			(uint32_t)i,
			nullptr,//�S�̈�փR�s�[
			img->pixels,//���f�[�^�A�h���X
			(uint32_t)img->rowPitch,//1���C���T�C�Y
			(uint32_t)img->slicePitch//�S�T�C�Y
		);
		assert(SUCCEEDED(result));
	}


	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(image.texBuff.Get(), &srvDesc, S_srvHandle);

	image.Ghandle = (int32_t)(S_srvHandle.ptr - S_CpuStartHandle);

	image.name = imgName;

	//��n���h����i�߂�
	S_srvHandle.ptr += S_incrementSize;

	ADXDataPool::SetImgDataPool(image);

	return image.Ghandle;
}

uint32_t ADXImage::CreateADXImage(const uint64_t& width, const uint64_t& height, const std::string& imgName)
{
	HRESULT result;
	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();

	ADXImage image;

	std::vector<ADXImage> imgDataPool = ADXDataPool::GetImgDataPool();

	ScratchImage mipChain{};
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	D3D12_RESOURCE_DESC textureResourceDesc{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����

	//�q�[�v�ݒ�
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureResourceDesc.Width = width;
	textureResourceDesc.Height = (uint32_t)height;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;
	textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//�e�N�X�`���o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&image.texBuff));

	const uint32_t pixelCount = (uint32_t)(width * height);
	const uint32_t rowPitch = sizeof(uint32_t) * (uint32_t)width;
	const uint32_t depthPitch = rowPitch * (uint32_t)height;
	std::vector<uint32_t> img = {};
	for (uint32_t i = 0; i < pixelCount; i++)
	{
		img.push_back(0xff0000ff);
	}

	result = image.texBuff->WriteToSubresource(
		0,
		nullptr,//�S�̈�փR�s�[
		&img.front(),//���f�[�^�A�h���X
		(uint32_t)rowPitch,//1���C���T�C�Y
		(uint32_t)depthPitch//�S�T�C�Y
	);


	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(image.texBuff.Get(), &srvDesc, S_srvHandle);

	image.Ghandle = (uint32_t)(S_srvHandle.ptr - S_CpuStartHandle);

	image.name = imgName;

	//��n���h����i�߂�
	S_srvHandle.ptr += S_incrementSize;

	ADXDataPool::SetImgDataPool(image);

	return image.Ghandle;
}