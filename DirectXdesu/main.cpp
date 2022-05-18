#include <stdio.h>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "KWindow.h"
#include "KDirectInit.h"
#include "KInput.h"
#ifdef DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")

using namespace DirectX;

// 定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // 色
};

// 定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	XMMATRIX mat; // 3D変換行列
};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef DEBUG
	// デバッグレイヤーをオンに
	ID3D12Ddebug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

#pragma region ウィンドウ
	KWindow win;
#pragma endregion

#pragma region DirectX初期化
	KDirectInit dx(win);
	KInput input(dx.result, win.window, win.handle);
#pragma endregion

#pragma region 描画初期化
#pragma region 深度バッファ
	// リソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = win.window_width; // レンダーターゲットに合わせる
	depthResourceDesc.Height = win.window_height; // レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // デプスステンシル
	// 深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	// リソース生成
	ID3D12Resource* depthBuff = nullptr;
	dx.result = dx.dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));
	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	dx.result = dx.dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx.dev->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
#pragma endregion

	// スケーリング倍率
	XMFLOAT3 scale;
	// 回転角
	XMFLOAT3 rotation;
	// 座標
	XMFLOAT3 pos;

	scale = { 1.0f,1.0f,1.0f };
	rotation = { 0.0f,0.0f,0.0f };
	pos = { 0.0f,0.0f,0.0f };

	// 速さ
	float speed = 0.0f;

	// 画像の色
	float colorR = 1.0f;
	float colorG = 1.0f;
	float colorB = 1.0f;
	float colorA = 1.0f;

	// カメラの距離
	float lenZ = -100;
#pragma region 頂点データ
	// 頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;	 // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;	 // uv座標
	};
	// 頂点データ
	Vertex vertices[] = {
		//  x	  y	   z	  u	   v
		// 前
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
		{{-5.0f, 5.0f,-5.0f},{},{0.0f,0.0f}}, // 左上
		{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,0.0f}}, // 右上
		// 後
		{{ 5.0f,-5.0f, 5.0f},{},{1.0f,1.0f}}, // 右下
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,1.0f}}, // 左下
		{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
		// 左
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
		{{-5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
		{{-5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
		// 右
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
		{{ 5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
		{{ 5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
		// 下
		{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
		{{ 5.0f,-5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
		// 上
		{{-5.0f, 5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
		{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}  // 右上
	};
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
#pragma region 頂点バッファ確保
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	dx.result = dx.dev->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(dx.result));
#pragma endregion
#pragma endregion

#pragma region インデックスデータ
	// インデックスデータ
	unsigned short indices[] = {
		// 前
		 0, 1, 2, // 三角形1つ目
		 2, 1, 3, // 三角形2つ目
		// 後
		 4, 5, 6,
		 6, 5, 7,
		 // 左
		  8, 9,10,
		 10, 9,11,
		 // 右
		 12,13,14,
		 14,13,15,
		 // 下
		 16,17,18,
		 18,17,19,
		 // 上
		 20,21,22,
		 22,21,23
	};
	// インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	dx.result = dx.dev->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	dx.result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// 全インデックスに対して
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	// マッピング解除
	indexBuff->Unmap(0, nullptr);
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
#pragma endregion

#pragma region 法線の計算
	for (int i = 0; i < 36 / 3; i++) {
		// 三角形１つごとに計算
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		// 三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);
		// p0 → p1ベクトル、p0 → p2ベクトルを計算 (ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		// 外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		// 正規化(長さを１にする)
		normal = XMVector3Normalize(normal);
		// 求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}
#pragma endregion

#pragma region 頂点バッファへのデータ転送
	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	dx.result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(dx.result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
#pragma endregion

#pragma region 頂点バッファビュー作成
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
#pragma endregion

#pragma region 頂点シェーダーファイルの読み込みとコンパイル
	ID3D10Blob* vsBlob = nullptr; // 頂点シェーダーオブジェクト
	ID3D10Blob* psBlob = nullptr; // ピクセルシェーダーオブジェクト
	ID3D10Blob* errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダーの読み込みとコンパイル
	dx.result = D3DCompileFromFile(
		L"BasicVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);
#pragma endregion

#pragma region シェーダーコードのエラー
	// エラーがでたら
	if (FAILED(dx.result)) {
		// erroeBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

#pragma region ピクセルシェーダの読み込みとコンパイル
	// ピクセルシェーダの読み込みとコンパイル
	dx.result = D3DCompileFromFile(
		L"BasicPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
#pragma endregion

#pragma region シェーダーコードのエラー
	// エラーがでたら
	if (FAILED(dx.result))
	{
		// erroeBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

#pragma region 頂点レイアウト
	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
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

#pragma region グラフィックスパイプライン設定
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineDesc.RasterizerState.DepthClipEnable = true;
	// ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// その他の設定
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc.SampleDesc.Count = 1;
	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	//// 加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;
	//// 加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_SUBTRACT;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;
	//// 反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;
	// 半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
#pragma region 定数バッファ
	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 定数バッファの生成
	ID3D12Resource* constBufferMaterial = nullptr;
	ID3D12Resource* constBuffTransform0 = nullptr;
	ID3D12Resource* constBuffTransform1 = nullptr;
	dx.result = dx.dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferMaterial));
	assert(SUCCEEDED(dx.result));
	// 定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;
	dx.result = constBufferMaterial->Map(
		0,
		nullptr,
		(void**)&constMapMaterial);
	assert(SUCCEEDED(dx.result));
	{
		// ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		// リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}
	// 定数バッファの生成
	dx.result = dx.dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform0));
	assert(SUCCEEDED(dx.result));
	dx.result = dx.dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(dx.result));
	// 定数バッファのマッピング
	dx.result = constBuffTransform0->Map(
		0,
		nullptr,
		(void**)&constMapTransform0);
	assert(SUCCEEDED(dx.result));
	dx.result = constBuffTransform1->Map(
		0,
		nullptr,
		(void**)&constMapTransform1);
	assert(SUCCEEDED(dx.result));
#pragma region 行列
	// ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, lenZ);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	float angleX = 0.0f; // カメラの回転角
	float angleY = 0.0f;
#pragma endregion

#pragma endregion
	// 値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(colorR, colorG, colorB, colorA);
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
	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = _countof(rootParam);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	dx.result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(dx.result));
	dx.result = dx.dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(dx.result));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;
	// デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true; // 深度テスト
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	// パイプラインステート
	ID3D12PipelineState* pipelineState = nullptr;
	dx.result = dx.dev->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(dx.result));
#pragma endregion
	// 横方向ピクセル数
	const size_t textureWidth = 256;
	// 縦方向ピクセル数
	const size_t textureHeight = 256;
	// 配列の要素数
	const size_t imageDataCount = textureWidth * textureHeight;

	TexMetadata metadata{};
	ScratchImage scraychImg{};

	dx.result = LoadFromWICFile(
		L"Resources/mario.jpg",
		WIC_FLAGS_NONE,
		&metadata, scraychImg);

	ScratchImage mipChain{};
	// ミニマップ作成
	dx.result = GenerateMipMaps(
		scraychImg.GetImages(),
		scraychImg.GetImageCount(),
		scraychImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0, mipChain
	);
	if (SUCCEEDED(dx.result)) {
		scraychImg = std::move(mipChain);
		metadata = scraychImg.GetMetadata();
	}
	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャバッファの作成
	ID3D12Resource* texBuff = nullptr;
	dx.result = dx.dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	// 全ミニマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// ミニマップレベルを指定してイメージを取得
		const Image* img = scraychImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		dx.result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(dx.result));
	}

	// SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// 設定を元にSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	dx.result = dx.dev->CreateDescriptorHeap(
		&srvHeapDesc,
		IID_PPV_ARGS(&srvHeap)
	);
	assert(SUCCEEDED(dx.result));

	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	// ハンドルの指す位置にシェーダーリソースビュー作成
	dx.dev->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
#pragma endregion

	// ウィンドウ表示
	// ゲームループ
	while (true)
	{
		win.Update();
		input.Update(dx.result);
#pragma region ウィンドウメッセージ
		if (win.breakFlag || input.IsPush(DIK_ESCAPE))
		{
			break;
		}
#pragma endregion

		// 更新
#pragma region DirectX毎フレーム処理
#pragma region キーボード処理
		// 背景色変え
		if (input.IsPush(DIK_SPACE)) {
			dx.bRed = 1.0f;
			dx.bGreen = 0.7f;
			dx.bBule = 1.0f;
		}
		else {
			dx.bRed = 0.1f;
			dx.bGreen = 0.25f;
			dx.bBule = 0.5f;
		}
		// 画像色変え
		if (input.IsPush(DIK_1)) {
			colorR = 1.0f;
			colorG = 0.0f;
			colorB = 1.0f;
		}
		else {
			colorR = 1.0f;
			colorG = 1.0f;
			colorB = 1.0f;
		}
		constMapMaterial->color = XMFLOAT4(colorR, colorG, colorB, colorA);
		//カメラ移動
		if (input.IsPush(DIK_D) || input.IsPush(DIK_A) ||
			input.IsPush(DIK_W) || input.IsPush(DIK_S)) {
			if (input.IsPush(DIK_D)) {
				angleX += XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_A)) {
				angleX -= XMConvertToRadians(1.0f);
			}

			if (input.IsPush(DIK_W)){
				angleY -= XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_S)) {
				angleY += XMConvertToRadians(1.0f);
			}

			// angleラジアンy軸回転
			eye.x = lenZ * sinf(angleX);
			eye.y = lenZ * sinf(angleY);
			eye.z = lenZ * cosf(angleX) * cosf(angleY);
		}
		//// 図形回転
		//if (input.IsPush(DIK_W) ||
		//	input.IsPush(DIK_S) ||
		//	input.IsPush(DIK_A) ||
		//	input.IsPush(DIK_D)) {
		//	if (input.IsPush(DIK_W)) {
		//		rotation.x = 1.0f;
		//	}
		//	else if (input.IsPush(DIK_S)) {
		//		rotation.x = -1.0f;
		//	}
		//	else {
		//		rotation.x = 0.0f;
		//	}

		//	if (input.IsPush(DIK_A)) {
		//		rotation.y = -1.0f;
		//	}
		//	else if (input.IsPush(DIK_D)) {
		//		rotation.y = 1.0f;
		//	}
		//	else {
		//		rotation.y = 0.0f;
		//	}
		//}
		//else {
		//	rotation.x = 0.0f;
		//	rotation.y = 0.0f;
		//	rotation.z = 0.0f;
		//}
		// 移動
		if (input.IsPush(DIK_UP) ||
			input.IsPush(DIK_DOWN) ||
			input.IsPush(DIK_RIGHT) ||
			input.IsPush(DIK_LEFT)) {
			speed = 1.0f;
			if (input.IsPush(DIK_UP)) {
				pos.z += speed;
			}
			if (input.IsPush(DIK_DOWN)) {
				pos.z -= speed;
			}
			if (input.IsPush(DIK_RIGHT)) {
				pos.x += speed;
			}
			if (input.IsPush(DIK_LEFT)) {
				pos.x -= speed;
			}
		}
		else {
			speed = 0.0f;
		}
#pragma endregion

#pragma region 行列
		// ビュー行列の計算
		matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		// 射影変換行列の計算
		XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),						// 上下画角45度
			(float)win.window_width / win.window_height,	// アスペクト比(画面横幅/画面縦幅)
			0.1f, 1000.0f									// 前端、奥端
		);

#pragma region 0番オブジェクトの毎フレーム処理
		// ワールド変換行列
		XMMATRIX matWorld0 = XMMatrixIdentity();
		// スケーリング行列
		XMMATRIX matScale0 = XMMatrixScaling(scale.x, scale.y, scale.z);
		// 回転行列
		XMMATRIX matRot0 = XMMatrixIdentity();
		// 平行移動行列
		XMMATRIX matTrans0 = XMMatrixIdentity(); // x,y,z移動

		matScale0 = XMMatrixScaling(scale.x, scale.y, scale.z); // スケーリングを行列の計算
		matRot0 *= XMMatrixRotationX(XMConvertToRadians(rotation.x)); // X軸
		matRot0 *= XMMatrixRotationY(XMConvertToRadians(rotation.y)); // Y軸
		matRot0 *= XMMatrixRotationZ(XMConvertToRadians(rotation.z)); // Z軸
		matTrans0 = XMMatrixTranslation(pos.x, pos.y, pos.z); // x,y,z移動

		// ワールド行列に乗算
		matWorld0 = XMMatrixIdentity();
		matWorld0 *= matScale0; // ワールド行列にスケーリングを乗算
		matWorld0 *= matRot0; // ワールド行列に回転を乗算
		matWorld0 *= matTrans0; // ワールド行列に平行移動を乗算

		// 定数バッファに転送
		constMapTransform0->mat = matWorld0 * matView * matProjection;
#pragma endregion

#pragma region 1番オブジェクトの毎フレーム処理
		// ワールド変換行列
		XMMATRIX matWorld1 = XMMatrixIdentity();
		// スケーリング行列
		XMMATRIX matScale1 = XMMatrixScaling(scale.x, scale.y, scale.z);
		// 回転行列
		XMMATRIX matRot1 = XMMatrixIdentity();
		// 平行移動行列
		XMMATRIX matTrans1 = XMMatrixIdentity(); // x,y,z移動

		matScale1 = XMMatrixScaling(1.0f, 1.0f, 1.0f); // スケーリングを行列の計算
		matRot1 *= XMMatrixRotationX(XMConvertToRadians(0)); // X軸
		matRot1 *= XMMatrixRotationY(XMConvertToRadians(XM_PI / 4.0f)); // Y軸
		matRot1 *= XMMatrixRotationZ(XMConvertToRadians(0)); // Z軸
		matTrans1 = XMMatrixTranslation(-20.0f, 0, 0); // x,y,z移動

		// ワールド行列に乗算
		matWorld1 = XMMatrixIdentity();
		matWorld1 *= matScale1; // ワールド行列にスケーリングを乗算
		matWorld1 *= matRot1; // ワールド行列に回転を乗算
		matWorld1 *= matTrans1; // ワールド行列に平行移動を乗算

		// 定数バッファに転送
		constMapTransform1->mat = matWorld1 * matView * matProjection;
#pragma endregion

#pragma endregion

#pragma endregion

		// 描画
#pragma region リソースバリア
		// バックバッファの番号を取得
		UINT bbIndex = dx.swapChain->GetCurrentBackBufferIndex();
		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = dx.backBuffers[bbIndex];
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		dx.cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region 描画先
		// 2. 描画先の変更
		// レンダーターゲートビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx.rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dx.dev->GetDescriptorHandleIncrementSize(dx.rtvHeapDesc.Type);
		// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		dx.cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region 画面クリア
		// 3. 画面クリア
		FLOAT clearColor[] = { dx.bRed,dx.bGreen,dx.bBule,0.0f };
		dx.cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		dx.cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
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
		dx.cmdList->RSSetViewports(1, &viewport);
#pragma endregion
#pragma region シザー矩形設定
		// シザー矩形
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// 切り抜き座標左
		scissorRect.right = scissorRect.left + win.window_width;	// 切り抜き座標右
		scissorRect.top = 0;									// 切り抜き座標上
		scissorRect.bottom = scissorRect.top + win.window_height;	// 切り抜き座標下
		// シザー矩形設定コマンドをコマンドリストに積む
		dx.cmdList->RSSetScissorRects(1, &scissorRect);
#pragma endregion
#pragma region パイプラインステート設定
		// パイプラインステートとルートシグネチャの設定コマンド
		dx.cmdList->SetPipelineState(pipelineState);
		dx.cmdList->SetGraphicsRootSignature(rootSignature);
#pragma endregion
#pragma region プリミティブ形状
		// プリミティブ形状の設定コマンド
		dx.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion
		// インデックスバッファビューの設定コマンド
		dx.cmdList->IASetIndexBuffer(&ibView);
#pragma region 頂点バッファビューの設定コマンド
		// 頂点バッファビューの設定コマンド
		dx.cmdList->IASetVertexBuffers(0, 1, &vbView);
#pragma endregion
		// CBV
		dx.cmdList->SetGraphicsRootConstantBufferView(0, constBufferMaterial->GetGPUVirtualAddress());
		// SRV
		dx.cmdList->SetDescriptorHeaps(1, &srvHeap);
		// 先頭ハンドルを取得
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRVヒープの先頭にあるSRVをルートパラメータ1番の設定
		dx.cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
#pragma region 描画コマンド
		// 描画コマンド
		// 0番定数バッファビューの設定コマンド
		dx.cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());
		dx.cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
		// 1番定数バッファビューの設定コマンド
		dx.cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
		dx.cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
#pragma endregion
		// 描画コマンドここまで
#pragma endregion
#pragma region リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		dx.cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion
#pragma region コマンドのフラッシュ
		// 命令のクローズ
		dx.result = dx.cmdList->Close();
		assert(SUCCEEDED(dx.result));
		// コマンドリストの実行
		ID3D12CommandList* cmdLists[] = { dx.cmdList };
		dx.cmdQueue->ExecuteCommandLists(1, cmdLists);
		// 画面に表示するバッファをフリップ(裏表の入れ替え)
		dx.result = dx.swapChain->Present(1, 0);
		assert(SUCCEEDED(dx.result));
#pragma endregion

#pragma region コマンド完了待ち
		// コマンドの完了を待つ
		dx.cmdQueue->Signal(dx.fence, ++dx.fenceVal);
		if (dx.fence->GetCompletedValue() != dx.fenceVal)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			dx.fence->SetEventOnCompletion(dx.fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
		// キューをクリア
		dx.result = dx.cmdAllocater->Reset();
		assert(SUCCEEDED(dx.result));
		// 再びコマンドを貯める準備
		dx.result = dx.cmdList->Reset(dx.cmdAllocater, nullptr);
		assert(SUCCEEDED(dx.result));
#pragma endregion
	}

	return 0;
}