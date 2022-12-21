#include "ADXimage.h"

D3D12_CPU_DESCRIPTOR_HANDLE ADXImage::srvHandle = {};
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ADXImage::srvHeap = nullptr;
UINT64 ADXImage::CpuStartHandle = 0;
ID3D12Device* ADXImage::device = nullptr;
UINT ADXImage::incrementSize = 0;
std::vector<ADXImage::imgNameAndGH> ADXImage::imgDataPool{};

ADXImage::ADXImage()
{

}

wchar_t* multiByteToWideChar(const std::string& pKey)
{
	const char* pCStrKey = pKey.c_str();
	//                �C     wchar_t*         
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
	wchar_t* pWCStrKey = new wchar_t[pSize];
	//                     
	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
	return pWCStrKey;
}

void ADXImage::StaticInitialize(ID3D12Device* setDevice)
{
	HRESULT result;

	device = setDevice;

	//�V�F�[�_�[���\�[�X�r���[�̍ő��
	const size_t kMaxSRVCount = 2056;
	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;
	//�ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRV�q�[�v�̐擪�n���h�����擾
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	CpuStartHandle = srvHandle.ptr;

	//�f�X�N���v�^�̃T�C�Y
	//�h���C�o�ɂ���ĈႤ�̂Ŋ֐��Ŏ擾���Ȃ���΂Ȃ�Ȃ�
	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

ADXImage ADXImage::LoadADXImage(std::string imgName)
{
	HRESULT result;

	ADXImage image;

	for (int i = 0; i < imgDataPool.size(); i++)
	{
		if (imgDataPool[i].imgName == imgName)
		{
			image.Ghandle = imgDataPool[i].GHandle;
			return image;
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
		multiByteToWideChar("Resources/" + imgName),
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
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
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
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = image.texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,//�S�̈�փR�s�[
			img->pixels,//���f�[�^�A�h���X
			(UINT)img->rowPitch,//1���C���T�C�Y
			(UINT)img->slicePitch//�S�T�C�Y
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
	device->CreateShaderResourceView(image.texBuff.Get(), &srvDesc, srvHandle);

	image.Ghandle = srvHandle.ptr - CpuStartHandle;

	//��n���h����i�߂�
	srvHandle.ptr += incrementSize;

	imgDataPool.push_back({imgName,image.Ghandle});

	return image;
}

int ADXImage::GetGHandle()
{
	return Ghandle;
}

ID3D12DescriptorHeap* ADXImage::GetSrvHeap()
{
	return srvHeap.Get();
}