#include "ADXimage.h"
#include "ADXDataPool.h"
#include "ADXCommon.h"
#include "ADXUtility.h"
#include <DirectXTex.h>

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

	//シェーダーリソースビューの最大個数
	const uint32_t kMaxSRVCount = 2056;
	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;
	//設定を元にSRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&S_srvHeap));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	S_srvHandle = S_srvHeap->GetCPUDescriptorHandleForHeapStart();
	S_CpuStartHandle = S_srvHandle.ptr;

	//デスクリプタのサイズ
	//ドライバによって違うので関数で取得しなければならない
	S_incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

uint32_t ADXImage::LoadADXImage(const std::string& imgName, bool generateMipMaps)
{
	HRESULT result;
	ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();

	ADXImage image;

	std::vector<ADXImage> imgDataPool = ADXDataPool::GetImgDataPool();

	for (int32_t i = 0; i < imgDataPool.size(); i++)
	{
		if (imgDataPool[i].name_ == imgName)
		{
			image.Ghandle_ = imgDataPool[i].Ghandle_;
			return image.Ghandle_;
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
		ADXUtility::StringToWideChar("Resources/" + imgName),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	size_t pos1 = 0;
	std::string fileExt = "";

	//区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = imgName.rfind('.');
	//検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{
		//区切り文字の後ろをファイル拡張子として保存
		fileExt = imgName.substr(pos1 + 1, imgName.size() - pos1 - 1);
	}

	if (generateMipMaps && fileExt != ".dds")
	{
		//ミップマップ生成
		result = GenerateMipMaps(
			scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
			TEX_FILTER_DEFAULT, 0, mipChain);
		if (SUCCEEDED(result))
		{
			scratchImg = std::move(mipChain);
			metadata = scratchImg.GetMetadata();
		}
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
	textureResourceDesc.Height = (uint32_t)metadata.height;
	textureResourceDesc.DepthOrArraySize = (uint16_t)metadata.arraySize;
	textureResourceDesc.MipLevels = (uint16_t)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&image.texBuff_));
	//全ミップマップについて
	for (uint32_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = image.texBuff_->WriteToSubresource(
			(uint32_t)i,
			nullptr,//全領域へコピー
			img->pixels,//元データアドレス
			(uint32_t)img->rowPitch,//1ラインサイズ
			(uint32_t)img->slicePitch//全サイズ
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
	device->CreateShaderResourceView(image.texBuff_.Get(), &srvDesc, S_srvHandle);

	image.Ghandle_ = (int32_t)(S_srvHandle.ptr - S_CpuStartHandle);

	image.name_ = imgName;

	//一つハンドルを進める
	S_srvHandle.ptr += S_incrementSize;

	ADXDataPool::SetImgDataPool(image);

	return image.Ghandle_;
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
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体

	//ヒープ設定
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureResourceDesc.Width = width;
	textureResourceDesc.Height = (uint32_t)height;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;
	textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//テクスチャバッファの生成
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&image.texBuff_));

	const uint32_t pixelCount = (uint32_t)(width * height);
	const uint32_t rowPitch = sizeof(uint32_t) * (uint32_t)width;
	const uint32_t depthPitch = rowPitch * (uint32_t)height;
	std::vector<uint32_t> img = {};
	for (uint32_t i = 0; i < pixelCount; i++)
	{
		img.push_back(0xff0000ff);
	}

	result = image.texBuff_->WriteToSubresource(
		0,
		nullptr,//全領域へコピー
		&img.front(),//元データアドレス
		(uint32_t)rowPitch,//1ラインサイズ
		(uint32_t)depthPitch//全サイズ
	);


	//シェーダーリソースビュー設定
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(image.texBuff_.Get(), &srvDesc, S_srvHandle);

	image.Ghandle_ = (uint32_t)(S_srvHandle.ptr - S_CpuStartHandle);

	image.name_ = imgName;

	//一つハンドルを進める
	S_srvHandle.ptr += S_incrementSize;

	ADXDataPool::SetImgDataPool(image);

	return image.Ghandle_;
}