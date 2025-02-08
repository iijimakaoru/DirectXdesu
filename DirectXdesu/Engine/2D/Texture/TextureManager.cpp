#include "TextureManager.h"
#include "KDirectXCommon.h"

TextureManager* TextureManager::textureManager = nullptr;
std::string TextureManager::DefaultTextureDirectoryPath = "Resource/";

void TextureManager::Init() {
	HRESULT result;

	device = KDirectXCommon::GetInstance()->GetDevice();
	commandList = KDirectXCommon::GetInstance()->GetCommandList();

	// デスクリプタレンジの設定
	descriptorRange.NumDescriptors = 1; // 1度の描画に使うテクスチャの数
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0; // レジスタ番号
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// SRV用デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	srvHeapDesc.NumDescriptors = static_cast<UINT>(MaxSRVCount); // テクスチャの枚数
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap)); // 生成
	assert(SUCCEEDED(result));

	// ヒープ設定
	textureHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
}

TextureData TextureManager::LoadTexture(const std::string& fileName) {

	//一回読み込んだことがあるファイルはそのまま返す
	auto textureItr = find_if(textures.begin(), textures.end(), [&](auto& texture)
		{
			return texture.second.path == fileName;
		});

	if (textureItr == textures.end())
	{
		HRESULT result;

		if (texCount > 1024)
		{
			assert(0);
		}

		TextureData data{};

		DirectX::TexMetadata metadata{};
		DirectX::ScratchImage scratchImg{};
		DirectX::ScratchImage mipChain{};

		data.srvHeap = srvHeap;
		data.descriptorRange = descriptorRange;

		wchar_t wfilepath[256];

		MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, wfilepath, _countof(wfilepath));

		// テクスチャロード
		result = LoadFromWICFile(wfilepath, DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);
		assert(SUCCEEDED(result));

		// ミニマップ作成
		result = GenerateMipMaps(
			scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
			DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

		if (SUCCEEDED(result))
		{
			scratchImg = std::move(mipChain);
			metadata = scratchImg.GetMetadata();
		}

		// 読み込んだディフューズテクスチャをSRGBとして扱う
		metadata.format = DirectX::MakeSRGB(metadata.format);

		// テクスチャバッファの生成
		data.texBuff = CreateTexBuff(metadata, scratchImg);

		// シェーダリソースビューの生成
		data.gpuHandle = CreateSRV(data.texBuff.Get(), metadata);

		// 横幅記憶
		data.width = metadata.width;

		// 縦幅記憶
		data.height = metadata.height;

		data.path = fileName;

		textures[fileName] = data;

		texCount++;

		return data;
	}
	else
	{
		return textures[fileName];

	}
}

TextureData TextureManager::LoadDivTexture(
    const std::string& fileName_, KMyMath::Vector2 leftTop_, KMyMath::Vector2 divSize_) {
	HRESULT result;

	if (texCount > 1024) {
		assert(0);
	}

	TextureData data{};

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	data.srvHeap = srvHeap;
	data.descriptorRange = descriptorRange;

	wchar_t wfilepath[256];

	MultiByteToWideChar(CP_ACP, 0, fileName_.c_str(), -1, wfilepath, _countof(wfilepath));

	// テクスチャロード
	result = LoadFromWICFile(wfilepath, DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	// ミニマップ作成
	result = GenerateMipMaps(
	    scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
	    DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = DirectX::MakeSRGB(metadata.format);

	// テクスチャバッファの生成
	data.texBuff = CreateTexBuff(metadata, scratchImg);

	// シェーダリソースビューの生成
	data.gpuHandle = CreateSRV(data.texBuff.Get(), metadata);

	// 横幅記憶
	data.width = metadata.width;

	// 縦幅記憶
	data.height = metadata.height;

	texCount++;

	textures[fileName_] = data;

	return data;
}

TextureData TextureManager::Load(const std::string& fileName) {
	return TextureManager::GetInstance()->LoadTexture(fileName);
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTexBuff(
    DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg) {

	KDirectXCommon::GetInstance()->BeginCommnd();

	std::vector<D3D12_SUBRESOURCE_DATA> textureSubresources;

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		D3D12_SUBRESOURCE_DATA subresouce{};

		subresouce.pData = scratchImg.GetImages()[i].pixels;
		subresouce.RowPitch = static_cast<LONG_PTR>(scratchImg.GetImages()[i].rowPitch);
		subresouce.SlicePitch = static_cast<LONG_PTR>(scratchImg.GetImages()[i].slicePitch);

		textureSubresources.push_back(subresouce);
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> result;
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, metadata.width, static_cast<UINT>(metadata.height), static_cast<UINT16>(metadata.arraySize), static_cast<UINT16>(metadata.mipLevels));


	//テクスチャバッファにデータ転送

	device->CreateCommittedResource(&textureHeapProp,D3D12_HEAP_FLAG_NONE,&textureResourceDesc,D3D12_RESOURCE_STATE_COPY_DEST,nullptr,IID_PPV_ARGS(&result));

	// ステージングバッファ準備
	UINT64 totalBytes = GetRequiredIntermediateSize(result.Get(), 0, static_cast<UINT>(textureSubresources.size()));

	Microsoft::WRL::ComPtr<ID3D12Resource> stagingBuffer;
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBytes);
	HRESULT hResult = device->CreateCommittedResource(&heapProps,D3D12_HEAP_FLAG_NONE,&resDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,IID_PPV_ARGS(&stagingBuffer));

	if (FAILED(hResult))
	{
		assert(0);
	}

	UpdateSubresources(commandList, result.Get(), stagingBuffer.Get(), 0, 0, static_cast<uint32_t>(textureSubresources.size()), textureSubresources.data());

	// コピー後にはテクスチャとしてのステートへ.
	KDirectXCommon::ResourceTransition(result.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	KDirectXCommon::GetInstance()->CloseCommnd();

	return result;
}

D3D12_GPU_DESCRIPTOR_HANDLE
TextureManager::CreateSRV(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata) {
	// アドレスの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	UINT incrementSize =
	    device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// ハンドルのポインタずらし
	cpuHandle.ptr += static_cast<UINT64>(texCount) * incrementSize;
	gpuHandle.ptr += static_cast<UINT64>(texCount) * incrementSize;

	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff, &srvDesc, cpuHandle);

	return gpuHandle;
}

void TextureManager::Delete() { delete textureManager; }

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

void TextureManager::LoadTextures() {
	
}

TextureData& TextureManager::GetTextures(std::string mapName)
{
	return textures[mapName];
}
