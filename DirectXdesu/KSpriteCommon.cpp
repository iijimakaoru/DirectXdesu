#include "KSpriteCommon.h"
#include <cassert>
#include "KShader.h"
#include "KMaterial.h"

void SpriteCommon::Init(KDirectXCommon* dxCommon)
{
	HRESULT result;

	assert(dxCommon);

	dxCommon_ = dxCommon;

	// 頂点データ
	vertices_ =
	{
		{{-0.5f, -0.5f,0.0f},{0.0f,1.0f}}, // 左下
		{{-0.5f, +0.5f,0.0f},{0.0f,0.0f}}, // 左上
		{{+0.5f, -0.5f,0.0f},{1.0f,1.0f}}, // 右下
		{{+0.5f, -0.5f,0.0f},{1.0f,1.0f}}, // 右下
		{{-0.5f, +0.5f,0.0f},{0.0f,0.0f}}, // 左上
		{{+0.5f, +0.5f,0.0f},{1.0f,0.0f}}, // 右上
	};

	// 
	indices_ =
	{
		0,1,2,
		1,2,3
	};

	// 
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;

	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB;
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dxCommon->GetDev()->CreateCommittedResource(
		&heapProp_,
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// 全頂点に対して
	for (int i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i];
	}

	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);

	// 
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();

	// 
	vbView_.SizeInBytes = sizeVB;

	// 
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	// 
	std::unique_ptr<KShader> shader = std::make_unique<KShader>();
	shader->SpritePSLoadCompile();
	shader->SpriteVSLoadCompile();
#pragma region 頂点レイアウト配列の宣言と設定
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{// xyz座標
			"POSITION",										// セマンティック名
			0,												// 同じセマンティック名が複数あるときに使うインデックス
			DXGI_FORMAT_R32G32B32_FLOAT,					// 要素数とビット数を表す
			0,												// 入力スロットインデックス
			D3D12_APPEND_ALIGNED_ELEMENT,					// データのオフセット
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// 入力データ種別
			0												// 一度に描画するインスタンス数
		},
		{// xyz座標
			"TEXCOORD",										// セマンティック名
			0,												// 同じセマンティック名が複数あるときに使うインデックス
			DXGI_FORMAT_R32G32_FLOAT,					// 要素数とビット数を表す
			0,												// 入力スロットインデックス
			D3D12_APPEND_ALIGNED_ELEMENT,					// データのオフセット
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// 入力データ種別
			0												// 一度に描画するインスタンス数
		},
	};
#pragma endregion
#pragma region パイプラインステート設定変数の宣言と各種項目の設定
	// グラフィックスパイプライン設定
	// シェーダーの設定
	pipelineDesc_.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = shader->psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// ラスタライザの設定
	pipelineDesc_.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // 背面をカリングしない
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineDesc_.RasterizerState.DepthClipEnable = true;

	// ブレンドステート
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc_.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.BlendEnable = true;
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// 半透明合成
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	// 頂点レイアウトの設定
	pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc_.NumRenderTargets = 1;
	pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc_.SampleDesc.Count = 1;

	// 
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 
	result = dxCommon_->GetDev()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result));

	// 
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));
	constMapMaterial->color = DirectX::XMFLOAT4(1, 0, 0, 1.0f);

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[2] = {};
	// 定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].Descriptor.ShaderRegister = 0;
	rootParams[0].Descriptor.RegisterSpace = 0;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// テクスチャサンブラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// ルートシグネチャの設定
	rootSignatureDesc_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc_.pParameters = rootParams;
	rootSignatureDesc_.NumParameters = _countof(rootParams);
	rootSignatureDesc_.pStaticSamplers = &samplerDesc;
	rootSignatureDesc_.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc_, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader->errorBlob);
	assert(SUCCEEDED(result));
	result = dxCommon->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	// 
	result = dxCommon->GetDev()->CreateGraphicsPipelineState(
		&pipelineDesc_,
		IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));
#pragma endregion

	// 
	TexMetadata metadata{};
	ScratchImage scratchImage{};

	// WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/haikei.jpg",
		WIC_FLAGS_NONE,
		&metadata,
		scratchImage);

	ScratchImage mipChain{};

	// ミニマップ生成
	result = GenerateMipMaps(scratchImage.GetImages(),
		scratchImage.GetImageCount(),
		scratchImage.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain);
	if (SUCCEEDED(result))
	{
		scratchImage = std::move(mipChain);
		metadata = scratchImage.GetMetadata();
	}

	// 読み込んだディヒューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// 
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// 
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// 
	result = dxCommon_->GetDev()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff_));

	// 
	// 全ミニマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) 
	{
		// ミニマップレベルを指定してイメージを取得
		const Image* img = scratchImage.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = texBuff_->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}

	// 
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// 
	result = dxCommon_->GetDev()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// 
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();

	// 
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// 
	dxCommon_->GetDev()->CreateShaderResourceView(texBuff_.Get(), &srvDesc, srvHandle);
}

void SpriteCommon::Draw()
{
	dxCommon_->GetCmdlist()->SetPipelineState(pipelineState_.Get());
	dxCommon_->GetCmdlist()->SetGraphicsRootSignature(rootSignature_.Get());

	dxCommon_->GetCmdlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dxCommon_->GetCmdlist()->IASetVertexBuffers(0, 1, &vbView_);

	// 定数バッファビュー(CBV)の設定コマンド
	dxCommon_->GetCmdlist()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());

	// デスクリプタヒープ配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	dxCommon_->GetCmdlist()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// SRVヒープの先頭ハンドルを所得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap_->GetGPUDescriptorHandleForHeapStart();

	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	dxCommon_->GetCmdlist()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	dxCommon_->GetCmdlist()->DrawInstanced(vertices_.size(), 1, 0, 0);
}
