#include "KTexture.h"

KTexture::KTexture(ID3D12Device* dev) {
	LoadTexture();
	GeneMipMap();
	SetTextureBuff();
	GeneTextureBuff(dev);
	SendData();
	SetDescHeap();
	GeneDescHeap(dev);
	GetSrvHandle(dev);
	SetSRV();
	CreateSRV(dev);
}

void KTexture::LoadTexture() {
	result = LoadFromWICFile(
		L"Resources/IMG_0150.JPG",
		WIC_FLAGS_NONE,
		&metadata, scraychImg);

	result = LoadFromWICFile(
		L"Resources/mario.jpg",
		WIC_FLAGS_NONE,
		&metadata2, scraychImg2);
}

void KTexture::GeneMipMap() {
	// ミニマップ作成
	result = GenerateMipMaps(
		scraychImg.GetImages(),
		scraychImg.GetImageCount(),
		scraychImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0, mipChain);

	if (SUCCEEDED(result)) {
		scraychImg = std::move(mipChain);
		metadata = scraychImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// ミニマップ作成
	result = GenerateMipMaps(
		scraychImg2.GetImages(),
		scraychImg2.GetImageCount(),
		scraychImg2.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0, mipChain2);

	if (SUCCEEDED(result)) {
		scraychImg2 = std::move(mipChain2);
		metadata2 = scraychImg2.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata2.format = MakeSRGB(metadata2.format);
}

void KTexture::SetTextureBuff() {
	// ヒープ設定
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// リソース設定
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc2.Format = metadata2.format;
	textureResourceDesc2.Width = metadata2.width;
	textureResourceDesc2.Height = (UINT)metadata2.height;
	textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResourceDesc2.SampleDesc.Count = 1;
}

void KTexture::GeneTextureBuff(ID3D12Device* dev) {
	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2));
}

void KTexture::SendData() {
	// 全ミニマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// ミニマップレベルを指定してイメージを取得
		const Image* img = scraychImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}

	// 全ミニマップについて
	for (size_t i = 0; i < metadata2.mipLevels; i++) {
		// ミニマップレベルを指定してイメージを取得
		const Image* img = scraychImg2.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = texBuff2->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}
}

void KTexture::SetDescHeap() {
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;
}

void KTexture::GeneDescHeap(ID3D12Device* dev) {
	result = dev->CreateDescriptorHeap(
		&srvHeapDesc,
		IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
}

void KTexture::GetSrvHandle(ID3D12Device* dev) {
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	srvHandle2 = srvHandle;
	incrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle2.ptr += incrementSize;
}

void KTexture::SetSRV() {
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	srvDesc2.Format = texBuff2->GetDesc().Format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = texBuff2->GetDesc().MipLevels;
}

void KTexture::CreateSRV(ID3D12Device* dev) {
	// ハンドルの指す位置にシェーダーリソースビュー作成
	dev->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
	dev->CreateShaderResourceView(texBuff2, &srvDesc2, srvHandle2);
}