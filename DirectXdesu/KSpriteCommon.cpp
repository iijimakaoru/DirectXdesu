#include "KSpriteCommon.h"
#include <cassert>
#include "KShader.h"
#include "ConstBuffer.h"

void SpriteCommon::Init(KDirectXCommon* dxCommon)
{
	HRESULT result;

	assert(dxCommon);

	dxCommon_ = dxCommon;

	spriteCommond_ = SpriteCommonCreate();
}

void SpriteCommon::BeginDraw()
{
	SpriteCommonBeginDraw();
}

PipelineSet SpriteCommon::SpriteCreateGraphicsPipeline()
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
	result = dxCommon_->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
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

	result = dxCommon_->GetDev()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

	return pipelineSet;
}

void SpriteCommon::SpriteCommonBeginDraw()
{
	// パイプラインステートの設定
	dxCommon_->GetCmdlist()->SetPipelineState(spriteCommond_.pipelineSet.pipelineState.Get());
	// ルートシグネチャの設定
	dxCommon_->GetCmdlist()->SetGraphicsRootSignature(spriteCommond_.pipelineSet.rootSignature.Get());
	// プリミティブ形状を設定
	dxCommon_->GetCmdlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// テクスチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { spriteCommond_.descHeap.Get() };
	dxCommon_->GetCmdlist()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

SpriteCommond SpriteCommon::SpriteCommonCreate()
{
	HRESULT result = S_FALSE;

	SpriteCommond spriteCommon{};

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;

	result = dxCommon_->GetDev()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&spriteCommon.descHeap));

	spriteCommon.pipelineSet = SpriteCreateGraphicsPipeline();

	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)KWinApp::window_width, (float)KWinApp::window_height, 0.0f, 0.0f, 1.0f);

	return spriteCommon;
}

HRESULT SpriteCommon::SpriteCommonLoadTexture(UINT texnumber, const wchar_t* filename)
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
	spriteCommond_.texBuff[texnumber] = nullptr;
	// テクスチャバッファの生成
	result = dxCommon_->GetDev()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteCommond_.texBuff[texnumber]));
#pragma endregion
#pragma region テクスチャバッファにデータ転送
	// 全ミニマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// ミニマップレベルを指定してイメージを取得
		const Image* img = scraychImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = spriteCommond_.texBuff[texnumber]->WriteToSubresource(
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
	result = dxCommon_->GetDev()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
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
	dxCommon_->GetDev()->CreateShaderResourceView(spriteCommond_.texBuff[texnumber].Get(), &srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(spriteCommond_.descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber,
			dxCommon_->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	return S_OK;
}