#include "PostEffect.h"
#include "KInput.h"

// 静的メンバ変数の実体
KMyMath::Matrix4 PostEffect::matPro;
KGPlin* PostEffect::pipeline = nullptr;
ComPtr<ID3D12Device> PostEffect::device;
ComPtr<ID3D12GraphicsCommandList> PostEffect::cmdList;
KWinApp* PostEffect::window = nullptr;
const float PostEffect::clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.0f };

void PostEffect::StaticInit()
{
	device = KDirectXCommon::GetInstance()->GetDev();

	window = KWinApp::GetInstance();

	cmdList = KDirectXCommon::GetInstance()->GetCmdlist();

	float width = static_cast<float>(window->GetWindowSizeW());
	float height = static_cast<float>(window->GetWindowSizeH());

	matPro = MyMathUtility::MakeOrthogonalL(0.0f, width, height, 0.0f, 0.0f, 1.0f);
}

void PostEffect::Init()
{
	// 定数バッファマテリアル
	CreateCBMaterial();

	// 頂点
	CreateVertex();

	// 定数バッファトランスフォーム
	CreateCBTransform();

	// テクスチャ
	CreateTextureBuff();

	// 深度バッファ
	CreateDepthBuff();

	*constMapTransform = MyMathUtility::MakeIdentity();
}

void PostEffect::DrawCommand()
{
	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// SRVヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descHeapSRV->GetGPUDescriptorHandleForHeapStart();
	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// 定数バッファビュー(CBV)の設定コマンド(マテリアル)
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// 定数バッファビュー(CBV)の設定コマンド(トランスフォーム)
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
}

void PostEffect::PreDrawScene()
{
	for (size_t i = 0; i < 2; i++)
	{
		// リソースバリア変更
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdList->ResourceBarrier(1, &barrier);
	}

	// レンダーターゲットビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];
	for (size_t i = 0; i < 2; i++)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(i),
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}

	// 深度ステンシルビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	// レンダーターゲットをセット
	cmdList->OMSetRenderTargets(2, rtvHs, false, &dsvH);

	// ビューポート設定
	CD3DX12_VIEWPORT viewPorts[2];

	// シザリング矩形設定
	CD3DX12_RECT rects[2];

	for (size_t i = 0; i < 2; i++)
	{
		viewPorts[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<FLOAT>(window->GetWindowSizeW()), static_cast<FLOAT>(window->GetWindowSizeH()));
		rects[i] = CD3DX12_RECT(0, 0, static_cast<LONG>(window->GetWindowSizeW()), static_cast<LONG>(window->GetWindowSizeH()));
	}
	cmdList->RSSetViewports(2, viewPorts);
	cmdList->RSSetScissorRects(2, rects);

	// 背面クリア
	for (size_t i = 0; i < 2; i++)
	{
		cmdList->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}

	// 深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene()
{
	for (size_t i = 0; i < 2; i++)
	{
		// リソースバリアを変更
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		cmdList->ResourceBarrier(1, &barrier);
	}
}

void PostEffect::Draw()
{
	// パイプラインセット
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト

	if (KInput::GetInstance()->IsTrigger(DIK_0))
	{
		// デスクリプタヒープにSRV作成
		static size_t tex = 0;

		tex = (tex + 1) % 2;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		device->CreateShaderResourceView(texBuff[tex].Get(),
			&srvDesc,
			descHeapSRV->GetCPUDescriptorHandleForHeapStart());
	}

	// 描画の条件
	DrawCommand();

	// 描画コマンド
	cmdList->DrawInstanced(_countof(vertices_), 1, 0, 0);
}

void PostEffect::SetPipeline(KGPlin* pipeline_)
{
	pipeline = pipeline_;
}

void PostEffect::CreateCBMaterial()
{
	// 定数バッファ生成用
	D3D12_HEAP_PROPERTIES cbHeapProp{}; // ヒープの設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(KMyMath::Vector4) + 0xff) & ~0xff; // 256バイトアライメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(constBuffMaterial.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // マッピング
	assert(SUCCEEDED(result));

	// 値の代入
	*constMapMaterial = KMyMath::Vector4(1.0f, 0.5f, 0.5f, 1.0f);
}

void PostEffect::CreateVertex()
{
	// ヒープ設定
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの数
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * 4);

	// リソース設定
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファの設定
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	// 頂点データ
	Vertex vertices[4] =
	{
		{{-1.0f, 1.0f,0.0f},{0.0f,0.0f}}, // 左上
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f}}, // 左下
		{{ 1.0f, 1.0f,0.0f},{1.0f,0.0f}}, // 右上
		{{ 1.0f,-1.0f,0.0f},{1.0f,1.0f}}, // 右下
	};

	for (size_t i = 0; i < 4; i++)
	{
		vertices_[i] = vertices[i];
	}

	// 頂点マップ
	Vertex* vertMap = nullptr;

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices_, sizeof(vertices_));
		vertBuff->Unmap(0, nullptr);
	}
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(Vertex);
}

void PostEffect::CreateCBTransform()
{
	// 定数バッファ生成用
	D3D12_HEAP_PROPERTIES cbHeapProp{}; // ヒープの設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC cbResDesc{};
	cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResDesc.Width = (sizeof(KMyMath::Matrix4) + 0xff) & ~0xff; // 頂点データ全体のサイズ
	cbResDesc.Height = 1;
	cbResDesc.DepthOrArraySize = 1;
	cbResDesc.MipLevels = 1;
	cbResDesc.SampleDesc.Count = 1;
	cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(constBuffTransform.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // マッピング
	assert(SUCCEEDED(result));
}

void PostEffect::CreateTextureBuff()
{
	// テクスチャリソース設定
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		window->GetWindowSizeW(),
		(UINT)window->GetWindowSizeH(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	CD3DX12_CLEAR_VALUE clear = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	for (size_t i = 0; i < 2; i++)
	{
		// テクスチャバッファの生成
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clear,
			IID_PPV_ARGS(&texBuff[i])
		);
		assert(SUCCEEDED(result));

		{// テクスチャ赤クリア
			// 画素数 ウィンドウ横 × ウィンドウ縦 = 総ピクセル数
			const UINT pixcelCount = window->GetWindowSizeW() * window->GetWindowSizeH();

			// 画素1行分のデータサイズ
			const UINT rowPitch = sizeof(UINT) * window->GetWindowSizeW();

			// 画像全体のデータサイズ
			const UINT depthPitch = rowPitch * window->GetWindowSizeH();

			// 画像イメージ
			UINT* img = new UINT[pixcelCount];
			for (size_t j = 0; j < pixcelCount; j++)
			{
				img[j] = 0xff0000ff;
			}

			// テクスチャバッファにデータ転送
			result = texBuff[i]->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
			assert(SUCCEEDED(result));
			delete[] img;
		}
	}

	// SRVデスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;

	// SRV用デスクリプタヒープ生成
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	// SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	// デスクリプタヒープにSRV作成
	device->CreateShaderResourceView(texBuff[0].Get(), // ビューと関連付けるバッファ
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::CreateDepthBuff()
{
	// RTV用デスクリプタヒープ作成
	CreateRTVDescHeap();

	// DSV用デスクリプタヒープ生成
	CreateDSVDescHeap();
}

void PostEffect::CreateDSVDescHeap()
{
	// DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvDescHeapDesc{};
	dsvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvDescHeapDesc.NumDescriptors = 1;

	// DSV用デスクリプタヒープ生成
	result = device->CreateDescriptorHeap(&dsvDescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	// デスクリプタヒープにDSV生成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::CreateRTVDescHeap()
{
	// RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;

	// RTV用デスクリプタヒープ生成
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	// レンダーターゲットビュー設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

	// シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (size_t i = 0; i < 2; i++)
	{
		// デスクリプタヒープにRTV作成
		device->CreateRenderTargetView(texBuff[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(),static_cast<INT>(i),
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}

	// 深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			static_cast<UINT>(window->GetWindowSizeW()),
			static_cast<UINT>(window->GetWindowSizeH()),
			1, 0, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE clear = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	// 深度バッファの生成
	result = device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clear,
		IID_PPV_ARGS(&depthBuff)
	);
	assert(SUCCEEDED(result));
}
