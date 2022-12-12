#include "KTexture.h"
#include "KDirectXCommon.h"
#include <string>

void KTexture::CreateTexture(const std::string& directoryPath, const std::string& filename)
{
	LoadTexture(directoryPath, filename);
	GeneMipMap();
	SetTextureBuff();
	GeneTextureBuff();
	SendData();
	SetDescHeap();
	GeneDescHeap();
	GetSrvHandle();
	SetSRV();
	CreateSRV();
}

void KTexture::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	string filePath = directoryPath + filename;

	// ユニコード文字列に変換する
	wchar_t wFilePath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wFilePath, _countof(wFilePath));

	result = LoadFromWICFile(
		wFilePath,
		WIC_FLAGS_NONE,
		&metadata, scraychImg);
	assert(SUCCEEDED(result));
}

void KTexture::GeneMipMap() 
{
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
}

void KTexture::GeneTextureBuff() {
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));
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
			(UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}
}

void KTexture::SetDescHeap() {
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;
}

void KTexture::GeneDescHeap() {
	result = KDirectXCommon::GetInstance()->GetDev()->CreateDescriptorHeap(
		&srvHeapDesc,
		IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
}

void KTexture::GetSrvHandle() {
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	incrementSize = KDirectXCommon::GetInstance()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void KTexture::SetSRV() {
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;
}

void KTexture::CreateSRV() {
	// ハンドルの指す位置にシェーダーリソースビュー作成
	KDirectXCommon::GetInstance()->GetDev()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);
}