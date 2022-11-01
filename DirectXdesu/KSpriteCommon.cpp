#include "KSpriteCommon.h"
#include <cassert>
#include "KShader.h"

void SpriteCommon::Init(KDirectXCommon* dxCommon)
{
	HRESULT result;

	assert(dxCommon);

	dxCommon_ = dxCommon;
	// 頂点データ
	vertices =
	{
		{-0.5f, -0.5f,0.0f},
		{-0.5f, +0.5f,0.0f},
		{+0.5f, -0.5f,0.0f},
	};
	// 
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dxCommon->GetDev()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリを取得
	DirectX::XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
	// 
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 
	vbView.SizeInBytes = sizeVB;
	// 
	vbView.StrideInBytes = sizeof(DirectX::XMFLOAT3);
	// 
	std::unique_ptr<KShader> shader = std::make_unique<KShader>();
	shader->SpritePSLoadCompile();
	shader->SpriteVSLoadCompile();
#pragma region 頂点レイアウト配列の宣言と設定
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{// xyz座標
		"POSITION",										// セマンティック名
		0,												// 同じセマンティック名が複数あるときに使うインデックス
		DXGI_FORMAT_R32G32B32_FLOAT,					// 要素数とビット数を表す
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
	// 
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader->errorBlob);
	assert(SUCCEEDED(result));
	result = dxCommon->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootSignature.Get();
	// 
	result = dxCommon->GetDev()->CreateGraphicsPipelineState(
		&gpipeline,
		IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion
}

void SpriteCommon::Draw()
{
	
}
