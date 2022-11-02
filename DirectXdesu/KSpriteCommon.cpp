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
	vertices =
	{
		{{-0.5f, -0.5f,0.0f},{0.0f,1.0f}}, // 左下
		{{-0.5f, +0.5f,0.0f},{0.0f,0.0f}}, // 左上
		{{+0.5f, -0.5f,0.0f},{1.0f,1.0f}}, // 右下
		{{+0.5f, -0.5f,0.0f},{1.0f,1.0f}}, // 右下
		{{-0.5f, +0.5f,0.0f},{0.0f,0.0f}}, // 左上
		{{+0.5f, +0.5f,0.0f},{1.0f,0.0f}}, // 右上
	};
	// 
	indices =
	{
		0,1,2,
		1,2,3
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
	Vertex* vertMap = nullptr;
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
	vbView.StrideInBytes = sizeof(vertices[0]);
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
	pipelineDesc.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader->psBlob->GetBufferSize();
	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザの設定
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // 背面をカリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineDesc.RasterizerState.DepthClipEnable = true;
	// ブレンドステート
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
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
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// その他の設定
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc.SampleDesc.Count = 1;
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
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));
	// 
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));
	constMapMaterial->color = DirectX::XMFLOAT4(1, 0, 0, 0.5f);
	// 
	D3D12_ROOT_PARAMETER rootParam = {};
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam.Descriptor.ShaderRegister = 0;
	rootParam.Descriptor.RegisterSpace = 0;
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// 
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParam;
	rootSignatureDesc.NumParameters = 1;
	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader->errorBlob);
	assert(SUCCEEDED(result));
	result = dxCommon->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();
	// 
	result = dxCommon->GetDev()->CreateGraphicsPipelineState(
		&pipelineDesc,
		IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion
}

void SpriteCommon::Draw()
{
	dxCommon_->GetCmdlist()->SetPipelineState(pipelineState.Get());
	dxCommon_->GetCmdlist()->SetGraphicsRootSignature(rootSignature.Get());

	dxCommon_->GetCmdlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dxCommon_->GetCmdlist()->IASetVertexBuffers(0, 1, &vbView);

	dxCommon_->GetCmdlist()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	dxCommon_->GetCmdlist()->DrawInstanced(vertices.size(), 1, 0, 0);
}
