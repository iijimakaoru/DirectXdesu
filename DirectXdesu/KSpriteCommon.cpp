#include "KSpriteCommon.h"
#include <cassert>
#include "KShader.h"
#include "ConstBuffer.h"

void SpriteCommon::Init(KDirectXCommon* dxCommon)
{
	HRESULT result;

	assert(dxCommon);

	dxCommon_ = dxCommon;

	// 頂点データ
	vertices_ =
	{
		{{  0.0f,100.0f,  0.0f},{0.0f,1.0f}}, // 左下
		{{  0.0f,  0.0f,  0.0f},{0.0f,0.0f}}, // 左上
		{{100.0f,100.0f,  0.0f},{1.0f,1.0f}}, // 右下
		{{100.0f,100.0f,  0.0f},{1.0f,1.0f}}, // 右下
		{{  0.0f,  0.0f,  0.0f},{0.0f,0.0f}}, // 左上
		{{100.0f,  0.0f,  0.0f},{1.0f,0.0f}}, // 右上
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
	VertexPosUV* vertMap = nullptr;
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
		{// uv座標
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

	// マテリアル
	CreateConstBufferMaterial();

	// トランスフォーム
	CreateConstBufferTransform();

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
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
	// 定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[2].Descriptor.ShaderRegister = 1;
	rootParams[2].Descriptor.RegisterSpace = 0;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

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
		L"Resources/mario.jpg",
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

	// 定数バッファビュー(CBV)の設定コマンド
	dxCommon_->GetCmdlist()->SetGraphicsRootConstantBufferView(2, constBufferTransform_->GetGPUVirtualAddress());

	// 描画コマンド
	dxCommon_->GetCmdlist()->DrawIndexedInstanced(indices_.size(), 1, 0, 0, 0);
}

void SpriteCommon::CreateConstBufferMaterial()
{
	HRESULT result;

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

	// 色をシェーダーに送る
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);
}

void SpriteCommon::CreateConstBufferTransform()
{
	HRESULT result;

	// 
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&constBufferTransform_));
	assert(SUCCEEDED(result));

	// 
	ConstBufferDataTransform* constMapTransform = nullptr;
	result = constBufferTransform_->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	// 行列をシェーダーに送る
	constMapTransform->mat = XMMatrixIdentity();
	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / KWinApp::window_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / KWinApp::window_height;
	constMapTransform->mat.r[3].m128_f32[0] = -2.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 2.0f;
}


void SpriteCommon::SpriteTransferVertexBuffer(const SpriteInfo& sprite, const SpriteCommond& spriteCommon)
{
	HRESULT result = S_FALSE;

	VertexPosUV vertices[] = {
		{{},{0.0f,1.0f}}, // 左下
		{{},{0.0f,0.0f}}, // 左上
		{{},{1.0f,1.0f}}, // 右下
		{{},{1.0f,0.0f}}, // 右上
	};

	enum { LB, LT, RB, RT };

	float left = (0.0f - sprite.anchorpoint.x) * sprite.size.x;
	float right = (1.0f - sprite.anchorpoint.x) * sprite.size.x;
	float top = (0.0f - sprite.anchorpoint.y) * sprite.size.y;
	float bottom = (1.0f - sprite.anchorpoint.y) * sprite.size.y;

	if (sprite.isFlipX)
	{
		left = -left;
		right = -right;
	}

	if (sprite.isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,	  top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,	  top,0.0f };

	if (sprite.isKiridasi)
	{
		if (spriteCommon.texBuff[sprite.texNum])
		{
			// 
			D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[sprite.texNum]->GetDesc();

			float tex_left = sprite.texLeftTop.x / resDesc.Width;
			float tex_right = (sprite.texLeftTop.x + sprite.texSize.x) / resDesc.Width;
			float tex_top = sprite.texLeftTop.y / resDesc.Height;
			float tex_bottom = (sprite.texLeftTop.y + sprite.texSize.y) / resDesc.Height;

			vertices[LB].uv = { tex_left,tex_bottom };
			vertices[LT].uv = { tex_left,tex_top };
			vertices[RB].uv = { tex_right,tex_bottom };
			vertices[RT].uv = { tex_right,tex_top };
		}
	}

	// 頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
}

PipelineSet SpriteCommon::SpriteCreateGraphicsPipeline(ID3D12Device* dev)
{
	HRESULT result;
#pragma region シェーダー読み込みとコンパイル
	std::unique_ptr<KShader> shader;
	shader = std::make_unique<KShader>();
	shader->SpritePSLoadCompile();
	shader->SpriteVSLoadCompile();
#pragma endregion
#pragma region 頂点レイアウト配列の宣言と設定
	static D3D12_INPUT_ELEMENT_DESC inputLayout[2] = {
	{// xyz座標
		"POSITION",										// セマンティック名
		0,												// 同じセマンティック名が複数あるときに使うインデックス
		DXGI_FORMAT_R32G32B32_FLOAT,					// 要素数とビット数を表す
		0,												// 入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,					// データのオフセット
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// 入力データ種別
		0												// 一度に描画するインスタンス数
	},
	{// uv座標
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	};
#pragma endregion
#pragma region パイプラインステート設定変数の宣言と各種項目の設定
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	// シェーダーの設定
	gpipeline.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = shader->psBlob->GetBufferSize();
	// サンプルマスクの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザの設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // 背面をカリングしない
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;
	// ブレンドステート
	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// その他の設定
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpipeline.SampleDesc.Count = 1;
	// 深度ステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthEnable = false;
	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	// 半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParam[3] = {};
	// 定数バッファ0番
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[0].Descriptor.ShaderRegister = 0;
	rootParam[0].Descriptor.RegisterSpace = 0;
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// テクスチャレジスタ0番
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// 定数バッファ1番
	rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[2].Descriptor.ShaderRegister = 1;
	rootParam[2].Descriptor.RegisterSpace = 0;
	rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
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
#pragma endregion
	// パイプラインとルートシグネチャのセット
	PipelineSet pipelineSet;
#pragma region ルートシグネチャの生成
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = _countof(rootParam);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader->errorBlob);
	assert(SUCCEEDED(result));
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootSignature));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = pipelineSet.rootSignature.Get();
#pragma endregion
#pragma region グラフィックスパイプラインステートの生成
	// グラフィックスパイプラインステートの設定
	gpipeline.DepthStencilState.DepthEnable = true; // 深度テスト
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 書き込み許可
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さければ合格
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

	return pipelineSet;
}

SpriteInfo SpriteCommon::SpriteCreate(ID3D12Device* dev, int window_width, int window_height, UINT texNumber, const SpriteCommond& spriteCommon, Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;
	// 新しいスプライトを作る
	SpriteInfo sprite{};
	// 頂点データ
	VertexPosUV vertices[] =
	{
		// x	  y		 z		  u	   v
		{{  0.0f,100.0f,  0.0f},{0.0f,1.0f}}, // 左下 
		{{  0.0f,  0.0f,  0.0f},{0.0f,0.0f}}, // 左上
		{{100.0f,100.0f,  0.0f},{1.0f,1.0f}}, // 右下
		{{100.0f,  0.0f,  0.0f},{1.0f,0.0f}}, // 右上
	};
	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = static_cast<UINT>(sizeof(vertices));
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// テクスチャ番号コピー
	sprite.texNum = texNumber;

	// 頂点バッファ生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.vertBuff));

	// 指定番号の画像が読み込み済みなら
	if (spriteCommon.texBuff[sprite.texNum])
	{
		// 
		D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[sprite.texNum]->GetDesc();
		// 
		sprite.size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// 反転フラグコピー
	sprite.isFlipX = isFlipX;
	sprite.isFlipY = isFlipY;

	// アンカーポイントコピー
	sprite.anchorpoint = anchorpoint;

	// 頂点バッファデータ転送
	SpriteTransferVertexBuffer(sprite, spriteCommon);

	// 頂点バッファビューの作成
	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);
	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.constBuff)
	);
	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = sprite.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // 色指定
	// 平行投影行列
	constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
	sprite.constBuff->Unmap(0, nullptr);

	return sprite;
}

void SpriteCommon::SpriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdList, const SpriteCommond& spriteCommon)
{
	// パイプラインステートの設定
	cmdList->SetPipelineState(spriteCommon.pipelineSet.pipelineState.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootSignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// テクスチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { spriteCommon.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void SpriteCommon::SpriteDraw(const SpriteInfo& sprite, ID3D12GraphicsCommandList* cmdList,
	const SpriteCommond& spriteCommon, ID3D12Device* dev)
{
	// 非表示フラグ
	if (sprite.isInvisible)
	{
		return;
	}
	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	// 定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, sprite.constBuff->GetGPUVirtualAddress());
	// シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
			sprite.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	// ポリゴンの描画
	cmdList->DrawInstanced(4, 1, 0, 0);
}

SpriteCommond SpriteCommon::SpriteCommonCreate(ID3D12Device* dev, int window_width, int window_height)
{
	HRESULT result = S_FALSE;

	SpriteCommond spriteCommon{};

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;

	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&spriteCommon.descHeap));

	spriteCommon.pipelineSet = SpriteCreateGraphicsPipeline(dev);

	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	return spriteCommon;
}

void SpriteCommon::SpriteUpdate(SpriteInfo& sprite, const SpriteCommond& spriteCommon)
{
	// ワールド行列
	sprite.matWorld = XMMatrixIdentity();
	// Z軸回転
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	// 平行移動
	sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, sprite.position.z);
	// 定数バッファの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = sprite.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = sprite.matWorld * spriteCommon.matProjection;
	constMap->color = sprite.color;
	sprite.constBuff->Unmap(0, nullptr);
}

HRESULT SpriteCommon::SpriteCommonLoadTexture(SpriteCommond& spriteCommon, UINT texnumber, const wchar_t* filename, ID3D12Device* dev)
{
	assert(texnumber <= spriteSRVCount - 1);

	HRESULT result;
#pragma region WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scraychImg{};
	// 画像読み込み
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scraychImg);

	ScratchImage mipChain{};
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
#pragma endregion
#pragma region リソース設定
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;
	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
#pragma endregion
#pragma region テクスチャバッファの生成
	spriteCommon.texBuff[texnumber] = nullptr;
	// テクスチャバッファの生成
	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteCommon.texBuff[texnumber]));
#pragma endregion
#pragma region テクスチャバッファにデータ転送
	// 全ミニマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// ミニマップレベルを指定してイメージを取得
		const Image* img = scraychImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = spriteCommon.texBuff[texnumber]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}
#pragma endregion

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = spriteSRVCount;

	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// ハンドルの示す先にシェーダーリソースビュー作成
	dev->CreateShaderResourceView(spriteCommon.texBuff[texnumber].Get(), &srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(spriteCommon.descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	return S_OK;
}