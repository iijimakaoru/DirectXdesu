#include "KDirectXCommon.h"
#include <thread>

// 対応レベルの配列
D3D_FEATURE_LEVEL levels[] = 
{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0
};

void KDirectXCommon::Init()
{
	// FPS固定初期化
	InitFixFPS();

#ifdef _DEBUG
	// デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
	}
#endif
	// DXGIファクトリーの生成
	ComPtr<IDXGIFactory6> dxgiFactory;
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;

	// ここに特定のアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter;

	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
	{
		// 動的配列に追加
		adapters.push_back(tmpAdapter);
	}

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
			tmpAdapter = adapters[i].Get();
			break;
		}
	}

	// デバイス生成
	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++) {
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	// エラー時のブレイク
	tmpAdapter->Release();

	// コマンドアロケーター生成
	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocater));
	assert(SUCCEEDED(result));

	// コマンドリスト生成
	result = dev->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocater.Get(), nullptr,
		IID_PPV_ARGS(&cmdList));
	assert(SUCCEEDED(result));

	// コマンドキューを生成
	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	assert(SUCCEEDED(result));

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = KWinApp::GetWindowSizeW();
	swapChainDesc.Height = KWinApp::GetWindowSizeH();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// スワップチェーンの生成
	ComPtr<IDXGISwapChain1> swapchain1;
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(), KWinApp::GetHWND(), &swapChainDesc, nullptr, nullptr,
		&swapchain1);

	// IDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換
	swapchain1.As(&swapChain);
	assert(SUCCEEDED(result));

	// RTV用デスクリプタヒープの生成
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
	result = dev->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	assert(SUCCEEDED(result));

	backBuffers.resize(swapChainDesc.BufferCount);

	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// デスクリプタヒープのハンドルを取得
		rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle.ptr += i * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// レンダーターゲートビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// シェーダーの計算結果をSKGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲートビューの生成
		dev->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}

	// リソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = KWinApp::GetWindowSizeW(); // レンダーターゲットに合わせる
	depthResourceDesc.Height = KWinApp::GetWindowSizeH(); // レンダーターゲットに合わせる
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

	// 深度バッファ
	result = dev->CreateCommittedResource(
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

	//　深度ビュー用ヒープ作成
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	assert(SUCCEEDED(result));

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void KDirectXCommon::PreDraw()
{
#pragma region リソースバリア
	// バックバッファの番号を取得
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	// 1.リソースバリアで書き込み可能に変更
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	cmdList->ResourceBarrier(1, &GetInstance()->barrierDesc);
#pragma endregion

#pragma region 描画先
	// 2. 描画先の変更
	// レンダーターゲートビューのハンドルを取得
	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region 画面クリア
	// 3. 画面クリア
	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f };
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

#pragma region ビューポート設定コマンド
	// ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};
	viewport.Width = KWinApp::GetWindowSizeW();   // 横幅
	viewport.Height = KWinApp::GetWindowSizeH(); // 縦幅
	viewport.TopLeftX = 0;                 // 左上x
	viewport.TopLeftY = 0;				   // 左上y
	viewport.MinDepth = 0.0f;			   // 最小深度
	viewport.MaxDepth = 1.0f;			   // 最大深度
	// ビューポート設定コマンドをコマンドリストに積む
	cmdList->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region シザー矩形設定
	// シザー矩形
	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									// 切り抜き座標左
	scissorRect.right = scissorRect.left + KWinApp::GetWindowSizeW();	// 切り抜き座標右
	scissorRect.top = 0;									// 切り抜き座標上
	scissorRect.bottom = scissorRect.top + KWinApp::GetWindowSizeH();	// 切り抜き座標下
	// シザー矩形設定コマンドをコマンドリストに積む
	cmdList->RSSetScissorRects(1, &scissorRect);
#pragma endregion
}

void KDirectXCommon::PostDraw()
{
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
	ID3D12CommandList* cmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	// 画面に表示するバッファをフリップ(裏表の入れ替え)
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region コマンド完了待ち
	// コマンドの完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) 
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		if (event != 0)
		{
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
	// キューをクリア
	result = cmdAllocater->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドを貯める準備
	result = cmdList->Reset(cmdAllocater.Get(), nullptr);
	assert(SUCCEEDED(result));
#pragma endregion
	// FPS固定
	UpdateFixFPS();
}

void KDirectXCommon::InitFixFPS()
{
	// 現在時間の記録
	reference_ = std::chrono::steady_clock::now();
}

void KDirectXCommon::UpdateFixFPS()
{
	// 1/60秒のぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60秒よりわずかに短い時間 
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));
	// 現在時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間を取得する
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);
	// 1/60秒(よりわずかに短い時間)経ってない場合
	if (elapsed < kMinTime)
	{
		// 1/60秒経過するまで微小なスリープを繰り返す
		while (std::chrono::steady_clock::now() - reference_ < kMinTime)
		{
			// 1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	float nowTime = std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::steady_clock::now() - reference_).count() / 1000000.0f;

	if (nowTime > 0)
	{
		fps = (fps * 0.99f) + (0.01f / nowTime);
	}
	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

KDirectXCommon* KDirectXCommon::GetInstance()
{
	static KDirectXCommon instance;
	return &instance;
}
