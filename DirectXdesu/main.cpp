#include <stdio.h>
#include <d3dcompiler.h>
#include "KWindow.h"
#include "KDirectInit.h"
#include "KInput.h"
#include "KDepth.h"
#include "KTexture.h"
#include "KWorldTransform.h"
#include "ViewProjection.h"
//#include "KGPlin.h"
#include "KModel.h"
#include "KShader.h"
#ifdef _DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")
#include "Sound.h"
#include <d3dx12.h>
#include "ConstBuffer.h"
#include "PipelineSet.h"
#include "Vector4.h"

const int spriteSRVCount = 512;

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct Sprite
{
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//	頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// Z軸周りの回転角
	float rotation = 0.0f;
	// 座標
	Vector3 position = { 0,0,0 };
	// ワールド行列
	XMMATRIX matWorld;
	// 色
	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	// テクスチャ番号
	UINT texNum = 0;
};

struct SpriteCommon
{
	// パイプラインセット
	PipelineSet pipelineSet;
	// 射影行列
	XMMATRIX matProjection{};
	// テクスチャ用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャリソース(テクスチャバッファ)の配列
	ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
};

PipelineSet Create3DObjectGpipeline(ID3D12Device* dev)
{
	HRESULT result;
#pragma region シェーダー読み込みとコンパイル
	std::unique_ptr<KShader> shader;
	shader = std::make_unique<KShader>();
	shader->BasicPSLoadCompile();
	shader->BasicVSLoadCompile();
#pragma endregion
#pragma region 頂点レイアウト配列の宣言と設定
	static D3D12_INPUT_ELEMENT_DESC inputLayout[3] = {
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
		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
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
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面をカリング
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

PipelineSet SpriteCreateGraphicsPipeline(ID3D12Device* dev)
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

Sprite SpriteCreate(ID3D12Device* dev, int window_width, int window_height)
{
	HRESULT result = S_FALSE;
	// 新しいスプライトを作る
	Sprite sprite{};
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
	// 頂点バッファ生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.vertBuff)
	);
	// 頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
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

void SpriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdList, const SpriteCommon& spriteCommon)
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

void SpriteDraw(const Sprite& sprite, ID3D12GraphicsCommandList* cmdList,
	const SpriteCommon& spriteCommon, ID3D12Device* dev)
{
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

SpriteCommon SpriteCommonCreate(ID3D12Device* dev, int window_width, int window_height)
{
	HRESULT result = S_FALSE;

	SpriteCommon spriteCommon{};

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

void SpriteUpdate(Sprite& sprite, const SpriteCommon& spriteCommon)
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

HRESULT SpriteCommonLoadTexture(SpriteCommon& spriteCommon,
	UINT texnumber, const wchar_t* filename, ID3D12Device* dev)
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
		CD3DX12_CPU_DESCRIPTOR_HANDLE(spriteCommon.descHeap->GetCPUDescriptorHandleForHeapStart(),texnumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	return S_OK;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	// デバッグレイヤーをオンに
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
	}
#endif
	HRESULT result;

#pragma region ウィンドウ
	KWindow win;
#pragma endregion
#pragma region DirectX初期化
	KDirectInit dx(win);
	//#ifdef _DEBUG
	//	ID3D12InfoQueue* infoQueue;
	//	if (SUCCEEDED(dx.dev->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
	//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
	//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	//		infoQueue->Release();
	//	}
	//#endif // _DEBUG
	KInput input(win.window, win.handle);
#pragma endregion
#pragma region 描画初期化
#pragma region 深度バッファ
	KDepth depth(dx.SetDev().Get(), win);
#pragma endregion
	// 速さ
	float speed = 1.0f;
#pragma region モデル
	KModel triangle = Triangle();
	triangle.CreateModel(dx.SetDev().Get());
	KModel cube = Cube();
	cube.CreateModel(dx.SetDev().Get());
	KModel line = Line();
	line.CreateModel(dx.SetDev().Get());
#pragma endregion
#pragma region テクスチャ初期化
	const wchar_t* msg = L"Resources/mario.jpg";
	const wchar_t* msg2 = L"Resources/iijan.jpg";
	const wchar_t* msg3 = L"Resources/haikei.jpg";
	const wchar_t* msg4 = L"Resources/kitanai.jpg";
	KTexture texture(dx.SetDev().Get(), msg, msg3);
	KTexture texture2(dx.SetDev().Get(), msg2, msg4);
#pragma endregion

#pragma region グラフィックスパイプライン設定
	// 3Dオブジェクト用パイプライン生成
	PipelineSet object3dPipelineSet = Create3DObjectGpipeline(dx.SetDev().Get());
	// スプライト用パイプライン生成
	PipelineSet spritePipelineSet = SpriteCreateGraphicsPipeline(dx.SetDev().Get());
#pragma region 3Dオブジェクト初期化
	const int ObjectNum = 2;
	const int LineNum = 6;
	// 3Dオブジェクト
	KWorldTransform object3d[ObjectNum];
	//KWorldTransform lineObject[LineNum];
	for (int i = 0; i < ObjectNum; i++) {
		object3d[i].Initialize(*dx.SetDev().Get());
		if (i > 0) {
			object3d[i].material->colorR = object3d[i].material->colorG = object3d[i].material->colorB = 1.0f;
		}
	}
	object3d[0].SetModel(&cube);
	object3d[0].SetTexture(&texture);
	object3d[1].SetModel(&cube);
	object3d[1].SetTexture(&texture2);
	/*for (int i = 0; i < LineNum; i++) {
		lineObject[i].Initialize(*dx.dev);
		lineObject[i].SetModel(&line);
	}
	lineObject[1].transform.pos.x = lineObject[0].transform.pos.x + 20;
	lineObject[2].transform.pos.x = lineObject[0].transform.pos.x - 20;
	lineObject[3].transform.rot.y = XMConvertToRadians(90);
	lineObject[4].transform.rot.y = XMConvertToRadians(90);
	lineObject[4].transform.pos.z = lineObject[3].transform.pos.z + 20;
	lineObject[5].transform.rot.y = XMConvertToRadians(90);
	lineObject[5].transform.pos.z = lineObject[3].transform.pos.z - 20;*/
#pragma endregion
#pragma region ビュー
	// ビュープロジェクション
	ViewProjection* viewProjection;
	viewProjection = new ViewProjection(win.window_width, win.window_height);
#pragma endregion

#pragma endregion
	Vector3 center = { 0,0,1 };

	float rSpeed = -0.02f;
	float gSpeed = 0.02f;
	float bSpeed = -0.02f;
	float aSpeed = -0.02f;

	std::unique_ptr<Sound> sound;
	sound = std::make_unique<Sound>();
	sound->Init();

	SoundData soundData1 = sound->SoundLoadWave("Sound/fanfare.wav");

	SpriteCommon spriteCommon;
	spriteCommon = SpriteCommonCreate(dx.SetDev().Get(), win.window_width, win.window_height);

	SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/haikei.jpg", dx.SetDev().Get());
	SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/mario.jpg", dx.SetDev().Get());

	Sprite sprite[2];
	for (int i = 0; i < _countof(sprite); i++)
	{
		sprite[i] = SpriteCreate(dx.SetDev().Get(), win.window_width, win.window_height);
	}
	sprite[0].texNum = 0;
	sprite[1].texNum = 1;
	sprite[1].rotation = 45;
	sprite[1].position = { 1280 / 2, 720 / 2,0 };
	//#pragma region スプライト
	//	std::unique_ptr<Sprite> sprite;
	//	sprite = std::make_unique<Sprite>();
	//
	//	Sprite::SpriteCommon spriteCommon;
	//	spriteCommon = sprite->SpriteCommonCreate(dx.SetDev().Get());
	//#pragma endregion
	//	Sprite::SpriteBuff spriteBuff;
	//	spriteBuff = sprite->SpriteCreate(dx.SetDev().Get(), win.window_width, win.window_height);
#pragma endregion

	// ウィンドウ表示
	// ゲームループ
	while (true)
	{
#pragma region ウィンドウメッセージ
		if (win.breakFlag || input.IsPush(DIK_ESCAPE)) {
			sound->GetxAudio().Reset();
			sound->SoundUnLoad(&soundData1);
			break;
		}
#pragma endregion

		// 更新
#pragma region DirectX毎フレーム処理
		for (int i = 0; i < _countof(sprite); i++)
		{
			SpriteUpdate(sprite[i], spriteCommon);
		}
#pragma region ウィンドウアップデート
		win.Update();
#pragma endregion

#pragma region inputアップデート
		input.Update();
#pragma endregion

#pragma region キーボード処理
		// 背景色変え
		if (input.IsTriger(DIK_SPACE)) {
			sound->SoundPlayWave(sound->GetxAudio().Get(), soundData1);
		}
		// 画像色変え
		if (object3d[0].material->colorR <= 0 || object3d[0].material->colorR >= 1) {
			rSpeed *= -1;
		}
		if (object3d[0].material->colorG <= 0 || object3d[0].material->colorG >= 1) {
			gSpeed *= -1;
		}
		if (object3d[0].material->colorB <= 0 || object3d[0].material->colorB >= 1) {
			bSpeed *= -1;
		}
		if (object3d[0].material->colorA <= 0 || object3d[0].material->colorA >= 1) {
			aSpeed *= -1;
		}
		object3d[0].material->colorR += rSpeed;
		object3d[0].material->colorG += gSpeed;
		object3d[0].material->colorB += bSpeed;
		if (input.IsPush(DIK_X)) {
			object3d[0].material->colorA += aSpeed;
		}
		// 図形縦回転
		if (input.IsPush(DIK_C) ||
			input.IsPush(DIK_V)) {
			if (input.IsPush(DIK_C)) {
				object3d[0].transform.rot.z += 0.1f;
			}

			if (input.IsPush(DIK_V)) {
				object3d[0].transform.rot.z -= 0.1f;
			}
		}
		////カメラ移動
		if (input.IsPush(DIK_D) || input.IsPush(DIK_A) ||
			input.IsPush(DIK_W) || input.IsPush(DIK_S)) {
			if (input.IsPush(DIK_D)) {
				viewProjection->angleX += XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_A)) {
				viewProjection->angleX -= XMConvertToRadians(1.0f);
			}
			if (input.IsPush(DIK_W)) {
				viewProjection->angleY -= XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_S)) {
				viewProjection->angleY += XMConvertToRadians(1.0f);
			}
			// angleラジアンy軸回転
			viewProjection->eye.x = viewProjection->lenZ * sinf(viewProjection->angleX);
			viewProjection->eye.y = viewProjection->lenZ * sinf(viewProjection->angleY);
			viewProjection->eye.z = viewProjection->lenZ * cosf(viewProjection->angleX) * cosf(viewProjection->angleY);
		}
		// 横回転
		if (input.IsPush(DIK_RIGHT) ||
			input.IsPush(DIK_LEFT)) {
			if (input.IsPush(DIK_RIGHT)) {
				object3d[0].transform.rot.y -= 0.1f;
			}
			if (input.IsPush(DIK_LEFT)) {
				object3d[0].transform.rot.y += 0.1f;
			}
		}
		//// 移動
		//if (input.IsPush(DIK_UP) ||
		//	input.IsPush(DIK_DOWN)) {
		//	if (input.IsPush(DIK_UP)) {
		//		speed = 1;
		//	}
		//	if (input.IsPush(DIK_DOWN)) {
		//		speed = -1;
		//	}
		//}
		//else {
		//	speed = 0;
		//}
		// 前ベクトル
		object3d[0].rotResult.x = sin(object3d[0].transform.rot.y) * center.z;
		object3d[0].rotResult.z = cos(object3d[0].transform.rot.y) * center.z;
		// 移動
		/*object3d[0]->transform.pos.x += (speed)*object3d[0]->rotResult.x;
		object3d[0]->transform.pos.z += (speed)*object3d[0]->rotResult.z;*/
		object3d[0].transform.pos.x += (input.IsPush(DIK_RIGHT) - input.IsPush(DIK_LEFT)) * speed;
		object3d[0].transform.pos.z += (input.IsPush(DIK_UP) - input.IsPush(DIK_DOWN)) * speed;

		object3d[1].transform.pos.x = object3d[0].transform.pos.x + 15;
		object3d[1].transform.pos.z = object3d[0].transform.pos.z;
		object3d[1].transform.pos.y = object3d[0].transform.pos.y + 10;

		object3d[1].transform.rot = object3d[0].transform.rot;
#pragma endregion

#pragma region 画像色アップデート
		//material->Update();
#pragma endregion

#pragma region ビューのアップデート
		viewProjection->Update(win.window_width, win.window_height);
#pragma endregion

#pragma region 3Dオブジェクトのアップデート
		for (int i = 0; i < ObjectNum; i++) {
			object3d[i].Update(viewProjection->matView, viewProjection->matProjection);
		}
		/*for (int i = 0; i < LineNum; i++) {
			lineObject[i].Update(viewProjection->matView, viewProjection->matProjection);
		}*/
#pragma endregion

#pragma endregion

		// 描画
#pragma region リソースバリア
		// バックバッファの番号を取得
		UINT bbIndex = dx.SetSChain()->GetCurrentBackBufferIndex();
		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = dx.SetBackBuffers()[bbIndex].Get();
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		dx.SetCmdlist()->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region 描画先
		// 2. 描画先の変更
		// レンダーターゲートビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx.SetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dx.SetDev()->GetDescriptorHandleIncrementSize(dx.SetRtvHeapDesc().Type);
		// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
		dx.SetCmdlist()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region 画面クリア
		// 3. 画面クリア
		FLOAT clearColor[] = { dx.bRed,dx.bGreen,dx.bBule,0.0f };
		dx.SetCmdlist()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		dx.SetCmdlist()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

#pragma region 描画
		// 描画コマンドここから
#pragma region ビューポート設定コマンド
		// ビューポート設定コマンド
		D3D12_VIEWPORT viewport{};
		viewport.Width = win.window_width;   // 横幅
		viewport.Height = win.window_height; // 縦幅
		viewport.TopLeftX = 0;                 // 左上x
		viewport.TopLeftY = 0;				   // 左上y
		viewport.MinDepth = 0.0f;			   // 最小深度
		viewport.MaxDepth = 1.0f;			   // 最大深度
		// ビューポート設定コマンドをコマンドリストに積む
		dx.SetCmdlist()->RSSetViewports(1, &viewport);
#pragma endregion
#pragma region シザー矩形設定
		// シザー矩形
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// 切り抜き座標左
		scissorRect.right = scissorRect.left + win.window_width;	// 切り抜き座標右
		scissorRect.top = 0;									// 切り抜き座標上
		scissorRect.bottom = scissorRect.top + win.window_height;	// 切り抜き座標下
		// シザー矩形設定コマンドをコマンドリストに積む
		dx.SetCmdlist()->RSSetScissorRects(1, &scissorRect);
#pragma endregion
#pragma region パイプラインステート設定
		// パイプラインステートとルートシグネチャの設定コマンド
		dx.SetCmdlist()->SetPipelineState(object3dPipelineSet.pipelineState.Get());
		dx.SetCmdlist()->SetGraphicsRootSignature(object3dPipelineSet.rootSignature.Get());
#pragma endregion
#pragma region プリミティブ形状
		// プリミティブ形状の設定コマンド
		dx.SetCmdlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region 描画コマンド
		// 描画コマンド
		for (int i = 0; i < ObjectNum; i++) {
			if (!input.IsPush(DIK_SPACE))
			{
				object3d[i].Draw(dx.SetCmdlist());
			}
			else
			{
				object3d[i].SecoundDraw(dx.SetCmdlist());
			}
		}
		/*for (int i = 0; i < LineNum; i++) {
			lineObject[i].Draw(dx.cmdList);
		}*/
		// スプライト描画
		SpriteCommonBeginDraw(dx.SetCmdlist(), spriteCommon);
		for (int i = 0; i < _countof(sprite); i++)
		{
			SpriteDraw(sprite[i], dx.SetCmdlist(), spriteCommon, dx.SetDev().Get());
		}
		// 描画コマンドここまで
#pragma endregion
#pragma endregion
#pragma region リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		dx.SetCmdlist()->ResourceBarrier(1, &barrierDesc);
#pragma endregion
#pragma region コマンドのフラッシュ
		dx.CmdFlash();
#pragma endregion
#pragma region コマンド完了待ち
		dx.CmdClear();
#pragma endregion
	}

	return 0;
}