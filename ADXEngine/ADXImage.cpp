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
	//                ，     wchar_t*         
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

	//シェーダーリソースビューの最大個数
	const size_t kMaxSRVCount = 2056;
	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;
	//設定を元にSRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	CpuStartHandle = srvHandle.ptr;

	//デスクリプタのサイズ
	//ドライバによって違うので関数で取得しなければならない
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
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体

	//WICテクスチャのロード
	result = LoadFromWICFile(
		multiByteToWideChar("Resources/" + imgName),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	//ヒープ設定
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&image.texBuff));
	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = image.texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,//全領域へコピー
			img->pixels,//元データアドレス
			(UINT)img->rowPitch,//1ラインサイズ
			(UINT)img->slicePitch//全サイズ
		);
		assert(SUCCEEDED(result));
	}


	//シェーダーリソースビュー設定
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(image.texBuff.Get(), &srvDesc, srvHandle);

	image.Ghandle = srvHandle.ptr - CpuStartHandle;

	//一つハンドルを進める
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