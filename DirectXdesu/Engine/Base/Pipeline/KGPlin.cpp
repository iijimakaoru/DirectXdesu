#include "KGPlin.h"
#include "KDirectXCommon.h"

void KGPlin::SetShader(KShader shader)
{
	// Vertex
	piplineDesc.VS.pShaderBytecode = shader.GetVSBlob()->GetBufferPointer();
	piplineDesc.VS.BytecodeLength = shader.GetVSBlob()->GetBufferSize();

	// Pixcel
	piplineDesc.PS.pShaderBytecode = shader.GetPSBlob()->GetBufferPointer();
	piplineDesc.PS.BytecodeLength = shader.GetPSBlob()->GetBufferSize();
}

void KGPlin::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParam.ParameterType = type;
	rootParam.Descriptor.ShaderRegister = shaderRegister;
	rootParam.Descriptor.RegisterSpace = registerSpace;
	rootParam.ShaderVisibility = shaderVisibility;
}

void KGPlin::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, D3D12_DESCRIPTOR_RANGE pDescripterRange, UINT numDescripterRanges, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParam.ParameterType = type;
	rootParam.DescriptorTable.pDescriptorRanges = &pDescripterRange;
	rootParam.DescriptorTable.NumDescriptorRanges = numDescripterRanges;
	rootParam.ShaderVisibility = shaderVisibility;
}

void KGPlin::SetRootSignature(UINT rootParamNum)
{
	HRESULT result;

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descripterRange{};
	descripterRange.NumDescriptors = 1;
	descripterRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descripterRange.BaseShaderRegister = 0;
	descripterRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//// ルートパラメータ
	// 設定
	std::vector<D3D12_ROOT_PARAMETER> rootParams = {};
	rootParams.resize(rootParamNum + 1);
	SetRootParam(rootParams[0], D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, descripterRange, 1);
	for (size_t i = 0; i < rootParamNum; i++)
	{
		SetRootParam(rootParams[i + 1], D3D12_ROOT_PARAMETER_TYPE_CBV, static_cast<UINT>(i), 0);
	}

	//// サンプラー
	// テクスチャサンプラーの設定
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

	//// ルートシグネチャ
	// 設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = static_cast<UINT>(rootParams.size());
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// シリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, rootSigBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result));
	result = KDirectXCommon::GetInstance()->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
}

void KGPlin::SetScreenRootSignature()
{
}

void KGPlin::Blending(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, const int mord)
{
	//	共通設定
	if (mord != NONE) {
		piplineDesc.BlendState.AlphaToCoverageEnable = true;
		blendDesc.BlendEnable = true;
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	}
	else {
		piplineDesc.BlendState.AlphaToCoverageEnable = false;
	}

	switch (mord)
	{
	case ADD:
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_ONE;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case SUB:
		blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.SrcBlend = D3D12_BLEND_ONE;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case INV:
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	case ALPHA:
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	default:
		break;
	}
}

void KGPlin::CreatePipelineAll(KShader shader, std::string shaderName)
{
	HRESULT result;

	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDev();

	// シェーダー設定
	SetShader(shader);

	// Objシェーダー
	if (shaderName == "Obj")
	{
		D3D12_INPUT_ELEMENT_DESC inputLayout[] =
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
			{// 法線ベクトル
				"NORMAL",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				0
			},
			{// uv座標
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				0
			},
		};

		// サンプルマスクの設定
		piplineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ラスタライザの設定
		piplineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面をカリング
		piplineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン塗りつぶし
		piplineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピング

		// ブレンドステート
		piplineDesc.BlendState.RenderTarget[0];

		// レンダーターゲットのブレンド設定
		D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = piplineDesc.BlendState.RenderTarget[0];
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		// 半透明合成
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		// 頂点レイアウトの設定
		piplineDesc.InputLayout.pInputElementDescs = inputLayout;
		piplineDesc.InputLayout.NumElements = _countof(inputLayout);

		// 図形の形状設定
		piplineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// 深度ステンシルステート
		piplineDesc.DepthStencilState.DepthEnable = true; // 深度テスト
		piplineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 書き込み許可
		piplineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さければ合格
		piplineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
		
		// その他の設定
		piplineDesc.NumRenderTargets = 1;
		piplineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		piplineDesc.SampleDesc.Count = 1;

		// デスクリプタレンジの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange{};
		descriptorRange.NumDescriptors = 1; // 一度の描画で使うテクスチャ数
		descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange.BaseShaderRegister = 0; // テクスチャレジスタ0番
		descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// ルートパラメータの設定
		D3D12_ROOT_PARAMETER rootParams[4] = {};

		// テクスチャレジスタ0番
		rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // 種類
		rootParams[0].DescriptorTable.pDescriptorRanges = &descriptorRange;
		rootParams[0].DescriptorTable.NumDescriptorRanges = 1;					  // デスクリプタレンジ数
		rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // 全てのシェーダーから見える

		// 定数バッファ0番
		rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // 種類
		rootParams[1].Descriptor.ShaderRegister = 0;				  // 定数バッファ番号
		rootParams[1].Descriptor.RegisterSpace = 0;					  // デフォルト値
		rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // 全てのシェーダーから見える

		// 定数バッファ1番
		rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // 種類
		rootParams[2].Descriptor.ShaderRegister = 1;				  // 定数バッファ番号
		rootParams[2].Descriptor.RegisterSpace = 0;					  // デフォルト値
		rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // 全てのシェーダーから見える

		// 定数バッファ2番
		rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // 種類
		rootParams[3].Descriptor.ShaderRegister = 2;				  // 定数バッファ番号
		rootParams[3].Descriptor.RegisterSpace = 0;					  // デフォルト値
		rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // 全てのシェーダーから見える

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
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.pParameters = rootParams; // ルートパラメータの先頭アドレス
		rootSignatureDesc.NumParameters = _countof(rootParams); // ルートパラメータ数
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.NumStaticSamplers = 1;

		// ルートシグネチャのシリアライズ
		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;
		result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
			rootSigBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
		assert(SUCCEEDED(result));
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(result));

		// パイプラインにルートシグネチャをセット
		piplineDesc.pRootSignature = rootSignature.Get();

		result = device->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(&pipelineState));
		assert(SUCCEEDED(result));
	}
	// Spriteシェーダー
	else if (shaderName == "Sprite")
	{
#pragma region 頂点レイアウト配列の宣言と設定
		static D3D12_INPUT_ELEMENT_DESC inputLayout[] = 
		{
			{// xy座標
				"POSITION",										// セマンティック名
				0,												// 同じセマンティック名が複数あるときに使うインデックス
				DXGI_FORMAT_R32G32B32_FLOAT,					// 要素数とビット数を表す
				0,												// 入力スロットインデックス
				D3D12_APPEND_ALIGNED_ELEMENT,					// データのオフセット
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// 入力データ種別
				0												// 一度に描画するインスタンス数
			},
			{ // uv座標(1行で書いたほうが見やすい)
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
		};
#pragma endregion
#pragma region パイプラインステート設定変数の宣言と各種項目の設定
		// サンプルマスクの設定
		piplineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ラスタライザの設定
		piplineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // 背面をカリングしない
		piplineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		piplineDesc.RasterizerState.DepthClipEnable = true;

		// ブレンドステート
		piplineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		// 頂点レイアウトの設定
		piplineDesc.InputLayout.pInputElementDescs = inputLayout;
		piplineDesc.InputLayout.NumElements = _countof(inputLayout);

		// 図形の形状設定
		piplineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// その他の設定
		piplineDesc.NumRenderTargets = 1;
		piplineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		piplineDesc.SampleDesc.Count = 1;

		// 深度ステンシルステート
		piplineDesc.DepthStencilState.DepthEnable = true; // 深度テスト
		piplineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 書き込み許可
		piplineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さければ合格
		piplineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
		
		// レンダーターゲットのブレンド設定
		D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = piplineDesc.BlendState.RenderTarget[0];
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
		descriptorRange.NumDescriptors = 1; // 一度の描画で使うテクスチャ数
		descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange.BaseShaderRegister = 0; // テクスチャレジスタ0番
		descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// ルートパラメータの設定
		D3D12_ROOT_PARAMETER rootParams[3] = {};

		// 定数バッファ0番
		rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // 種類
		rootParams[0].Descriptor.ShaderRegister = 0;				  // 定数バッファ番号
		rootParams[0].Descriptor.RegisterSpace = 0;					  // デフォルト値
		rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // 全てのシェーダーから見える

		// テクスチャレジスタ0番
		rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // 種類
		rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
		rootParams[1].DescriptorTable.NumDescriptorRanges = 1;					  // デスクリプタレンジ数
		rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // 全てのシェーダーから見える

		// 定数バッファ1番
		rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // 種類
		rootParams[2].Descriptor.ShaderRegister = 1;				  // 定数バッファ番号
		rootParams[2].Descriptor.RegisterSpace = 0;					  // デフォルト値
		rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // 全てのシェーダーから見える

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
#pragma region ルートシグネチャの生成
		// ルートシグネチャの設定
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.pParameters = rootParams; // ルートパラメータの先頭アドレス
		rootSignatureDesc.NumParameters = _countof(rootParams); // ルートパラメータ数
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.NumStaticSamplers = 1;

		// ルートシグネチャのシリアライズ
		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;
		result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
			rootSigBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
		assert(SUCCEEDED(result));
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(result));

		// パイプラインにルートシグネチャをセット
		piplineDesc.pRootSignature = rootSignature.Get();
#pragma endregion
#pragma region グラフィックスパイプラインステートの生成
		result = device->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(&pipelineState));
		assert(SUCCEEDED(result));
#pragma endregion
	}
	// Fbxシェーダー
	else if (shaderName == "Fbx")
	{
		// 頂点レイアウト
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ // xy座標(1行で書いたほうが見やすい)
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // 法線ベクトル(1行で書いたほうが見やすい)
				"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // uv座標(1行で書いたほうが見やすい)
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{// 影響を受けるボーン番号(4つ)
				"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{// ボーンスキンウェイト(4つ)
				"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
		};

		// サンプルマスク
		piplineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

		// ラスタライザステート
		piplineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

		// デプスステンシルステート
		piplineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

		// レンダーターゲットのブレンド設定
		D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		// ブレンドステートの設定
		piplineDesc.BlendState.RenderTarget[0] = blenddesc;

		// 深度バッファのフォーマット
		piplineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// 頂点レイアウトの設定
		piplineDesc.InputLayout.pInputElementDescs = inputLayout;
		piplineDesc.InputLayout.NumElements = _countof(inputLayout);

		// 図形の形状設定（三角形）
		piplineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		piplineDesc.NumRenderTargets = 1;    // 描画対象は1つ
		piplineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA(SRGB版)
		piplineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

		// デスクリプタレンジ
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

		// ルートパラメータ
		CD3DX12_ROOT_PARAMETER rootparams[3] = {};

		// CBV(座標変換行列用)
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

		// SRV(テクスチャ)
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

		// CBV(スキニング用)
		rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

		// スタティックサンプラー
		CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

		// ルートシグネチャの設定
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;

		// バージョン自動判定のシリアライズ
		result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

		// ルートシグネチャの生成
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { assert(0); }

		piplineDesc.pRootSignature = rootSignature.Get();

		// グラフィックスパイプラインの生成
		result = device->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { assert(0); }
	}
	// PostEffectシェーダー
	else if (shaderName == "PostEffect")
	{
		D3D12_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{// xy座標
				"POSITION",										// セマンティック名
				0,												// 同じセマンティック名が複数あるときに使うインデックス
				DXGI_FORMAT_R32G32B32_FLOAT,					// 要素数とビット数を表す
				0,												// 入力スロットインデックス
				D3D12_APPEND_ALIGNED_ELEMENT,					// データのオフセット
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// 入力データ種別
				0												// 一度に描画するインスタンス数
			},
			{ // uv座標(1行で書いたほうが見やすい)
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
		};

		// サンプルマスク
		piplineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

		// ラスタライザステート
		piplineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		piplineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		// デプスステンシルステート
		piplineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		piplineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // 常に上書き

		// レンダーターゲットのブレンド設定
		D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		// ブレンドステートの設定
		piplineDesc.BlendState.RenderTarget[0] = blenddesc;

		// 深度バッファのフォーマット
		piplineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// 頂点レイアウトの設定
		piplineDesc.InputLayout.pInputElementDescs = inputLayout;
		piplineDesc.InputLayout.NumElements = _countof(inputLayout);

		// 図形の形状設定（三角形）
		piplineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		piplineDesc.NumRenderTargets = 1;    // 描画対象は1つ
		piplineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA(SRGB版)
		piplineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

		// デスクリプタレンジ
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

		// ルートパラメータ
		CD3DX12_ROOT_PARAMETER rootparams[3] = {};

		// CBV(座標変換行列用)
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

		// SRV(テクスチャ)
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

		// CBV(座標変換行列用)
		rootparams[2].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);

		// スタティックサンプラー
		CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

		// ルートシグネチャの設定
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;

		// バージョン自動判定のシリアライズ
		result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
		if (FAILED(result)) { assert(0); }

		// ルートシグネチャの生成
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { assert(0); }

		piplineDesc.pRootSignature = rootSignature.Get();

		// グラフィックスパイプラインの生成
		result = device->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { assert(0); }
	}
	// なんもシェーダー入ってないとき
	else
	{
		assert(0);
	}
}

void KGPlin::Update(D3D12_PRIMITIVE_TOPOLOGY primitive)
{
	// パイプラインステートとルートシグネチャの設定コマンド
	KDirectXCommon::GetInstance()->GetCmdlist()->SetPipelineState(pipelineState.Get());
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetPrimitiveTopology(primitive);
}

void KGPlin::Setting()
{
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootSignature(rootSignature.Get());
}

void KGPlin::SetBlending(int mord)
{
	HRESULT result;
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = piplineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blending(blenddesc, mord);
	result = KDirectXCommon::GetInstance()->GetDev()->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}
