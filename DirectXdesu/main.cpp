#include <Windows.h>
#include <stdio.h>
#include<vector>
#include<string>
#include "Window.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#ifdef DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.Lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

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

	// 変数
#pragma region ウィンドウ
	Window* win = new Window;
#pragma endregion

#pragma region DirectX初期化
	HRESULT result;
	ID3D12Device* dev = nullptr;
	IDXGIFactory6* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* cmdAllocater = nullptr;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	ID3D12CommandQueue* cmdQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	float bRed = 0.1f;
	float bGreen = 0.25f;
	float bBule = 0.5f;

	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

#pragma region アダプタ
	// アダプターの列挙用
	std::vector<IDXGIAdapter4*> adapters;
	// ここに特定のアダプターオブジェクトが入る
	IDXGIAdapter4* tmpAdapter = nullptr;

	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得
		adapters[i]->GetDesc3(&adapterDesc);

		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
	{
		// 動的配列に追加
		adapters.push_back(tmpAdapter);
	}
#pragma endregion

#pragma region デバイス
	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++)
	{
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
#pragma endregion

#pragma region コマンドリスト
	// コマンドアロケーター生成
	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocater));
	assert(SUCCEEDED(result));

	// コマンドリスト生成
	result = dev->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocater, nullptr,
		IID_PPV_ARGS(&cmdList));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region コマンドキュー
	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	// コマンドキューを生成
	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region スワップチェーン
	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	// スワップチェーンの生成
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue, win->handle, &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain);
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region デスクリプタヒープ
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
	// デスクリプタヒープの生成
	dev->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
#pragma endregion

#pragma region バックバッファ
	// バックバッファ
	std::vector<ID3D12Resource*> backBuffers;
	backBuffers.resize(swapChainDesc.BufferCount);
#pragma endregion

#pragma region レンダーターゲートビュー
	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle.ptr += i * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// レンダーターゲートビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// シェーダーの計算結果をSKGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲートビューの生成
		dev->CreateRenderTargetView(backBuffers[i], &rtvDesc, rtvHandle);
	}
#pragma endregion

#pragma region フェンス
	// フェンスの生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion

#pragma region DirectInputの初期化
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		win->window.hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		nullptr
	);
	assert(SUCCEEDED(result));
	// キーボードデバイスの生成
	IDirectInputDevice8* keyboad = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboad, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = keyboad->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = keyboad->SetCooperativeLevel(
		win->handle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));
#pragma endregion
#pragma endregion

#pragma region 描画初期化

	// 頂点データ
	XMFLOAT3 vertices[] = {
		{-0.5f,-0.5f,0.0f},// 左下
		{-0.5f,+0.5f,0.0f},// 左上
		{+0.5f,-0.5f,0.0f} // 右下
	};

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

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
	result = dev->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリを取得
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(XMFLOAT3);

	ID3D10Blob* vsBlob = nullptr; // 頂点シェーダーオブジェクト
	ID3D10Blob* psBlob = nullptr; // ピクセルシェーダーオブジェクト
	ID3D10Blob* errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);

	// エラーがでたら
	if (FAILED(result))
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

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	// エラーがでたら
	if (FAILED(result))
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

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

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
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
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

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	// パイプラインステート
	ID3D12PipelineState* pipelineState = nullptr;
	result = dev->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

	// ウィンドウ表示
	// ゲームループ
	while (true)
	{

#pragma region ウィンドウメッセージ
		win->Update();

		if (win->breakFlag)
		{
			break;
		}
#pragma endregion

#pragma region リソースバリア
		// バックバッファの番号を取得
		UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex];
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region 描画先
		// 2. 描画先の変更
		// レンダーターゲートビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
#pragma endregion

#pragma region 画面クリア
		// 3. 画面クリア
		FLOAT clearColor[] = { bRed,bGreen,bBule,0.0f };
		cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
#pragma endregion

#pragma region キーボード
		// キーボードの情報取得
		keyboad->Acquire();
		// 全キーの入力状態を所得
		BYTE key[256] = {};
		BYTE oldkey[256] = {};
		keyboad->GetDeviceState(sizeof(key), key);
		keyboad->GetDeviceState(sizeof(oldkey), oldkey);
		for (int i = 0; i < 256; ++i)
		{
			oldkey[i] = key[i];
		}

		if (key[DIK_SPACE]) {
			bRed = 1.0f;
			bGreen = 0.7f;
			bBule = 1.0f;
		}
		else {
			bRed = 0.1f;
			bGreen = 0.25f;
			bBule = 0.5f;
		}
#pragma endregion

#pragma region 描画
		// 描画コマンドここから
		// ビューポート設定コマンド
		D3D12_VIEWPORT viewport{};
		/*D3D12_VIEWPORT viewport{};
		D3D12_VIEWPORT viewport{};
		D3D12_VIEWPORT viewport{};*/
		viewport.Width = win->window_width - 500;   // 横幅
		viewport.Height = win->window_height - 500; // 縦幅
		viewport.TopLeftX = 0;                 // 左上x
		viewport.TopLeftY = 0;				   // 左上y
		viewport.MinDepth = 0.0f;			   // 最小深度
		viewport.MaxDepth = 1.0f;			   // 最大深度

		// ビューポート設定コマンドをコマンドリストに積む
		cmdList->RSSetViewports(1, &viewport);

		// シザー矩形
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// 切り抜き座標左
		scissorRect.right = scissorRect.left + win->window_width;	// 切り抜き座標右
		scissorRect.top = 0;									// 切り抜き座標上
		scissorRect.bottom = scissorRect.top +win-> window_height;	// 切り抜き座標下

		// シザー矩形設定コマンドをコマンドリストに積む
		cmdList->RSSetScissorRects(1, &scissorRect);

		// パイプラインステートとルートシグネチャの設定コマンド
		cmdList->SetPipelineState(pipelineState);
		cmdList->SetGraphicsRootSignature(rootSignature);

		// プリミティブ形状の設定コマンド
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 頂点バッファビューの設定コマンド
		cmdList->IASetVertexBuffers(0, 1, &vbView);

		// 描画コマンド
		cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
		// 描画コマンドここまで
#pragma endregion

#pragma region リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region コマンドのフラッシュ
		// 命令のクローズ
		result = cmdList->Close();
		assert(SUCCEEDED(result));
		// コマンドリストの実行
		ID3D12CommandList* cmdLists[] = { cmdList };
		cmdQueue->ExecuteCommandLists(1, cmdLists);

		// 画面に表示するバッファをフリップ(裏表の入れ替え)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));
#pragma endregion

#pragma region コマンド完了待ち
		// コマンドの完了を待つ
		cmdQueue->Signal(fence, ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		// キューをクリア
		result = cmdAllocater->Reset();
		assert(SUCCEEDED(result));
		// 再びコマンドを貯める準備
		result = cmdList->Reset(cmdAllocater, nullptr);
		assert(SUCCEEDED(result));
#pragma endregion
	}

	return 0;
}