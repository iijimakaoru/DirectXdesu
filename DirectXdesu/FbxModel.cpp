#include "FbxModel.h"

void FbxModel::CreateBuffer()
{
	HRESULT result;

	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDev();

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUVSkin) * vertices.size());

	CD3DX12_HEAP_PROPERTIES heap1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	CD3DX12_RESOURCE_DESC vB = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	result = device->CreateCommittedResource(
		&heap1,
		D3D12_HEAP_FLAG_NONE,
		&vB,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	VertexPosNormalUVSkin* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(vertices.begin(), vertices.end(), vertMap);
	vertBuff->Unmap(0, nullptr);

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	// インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	CD3DX12_RESOURCE_DESC iB = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	result = device->CreateCommittedResource(
		&heap1,
		D3D12_HEAP_FLAG_NONE,
		&iB,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	std::copy(indices.begin(), indices.end(), indexMap);
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//vertexs.reset(new KVertex(KDirectXCommon::GetInstance()->GetDev(), vertices, indices));

	// テクスチャ画像データ
	const Image* img = scratchImg.GetImage(0, 0, 0);
	assert(img);

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// リソース設定
	D3D12_RESOURCE_DESC textureDesc{};
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Format = metadata.format;
	textureDesc.Width = metadata.width;
	textureDesc.Height = (UINT)metadata.height;
	textureDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureDesc.SampleDesc.Count = 1;

	CD3DX12_RESOURCE_DESC textureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels
		);

	CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	// テクスチャバッファ生成
	result = device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	result = texBuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);

	// SRV用デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	srvHeapDesc.NumDescriptors = 1; // テクスチャの枚数
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap)); // 生成
	assert(SUCCEEDED(result));

	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHeap->GetCPUDescriptorHandleForHeapStart());
}

void FbxModel::LoadTexture()
{
	//texture.CreateTexture("Resources/", name);
}

void FbxModel::Draw()
{
	ID3D12GraphicsCommandList* cmdList = KDirectXCommon::GetInstance()->GetCmdlist();

	// 頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// インデックスバッファビューの設定
	cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープのセット
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, srvHeap->GetGPUDescriptorHandleForHeapStart());

	// 描画
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

FbxModel::~FbxModel()
{
	// Fbxシーンの解放
	fbxScene->Destroy();
}
