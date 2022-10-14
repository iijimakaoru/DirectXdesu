#include "KDirectXCommon.h"

KDirectXCommon::KDirectXCommon(KWinApp window) {
	bRed = 0.1f;
	bGreen = 0.25f;
	bBule = 0.5f;

	DXGIFactory();

	Adapter();

	CommandList();

	CommandQueue();

	SwapChain(window);

	Descriptor();

	BackBuffer();

	Fence();
}

void KDirectXCommon::PreDraw(ID3D12DescriptorHeap* dsvHeap, int window_width, int window_height)
{
#pragma region リソースバリア
	// バックバッファの番号を取得
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	// 1.リソースバリアで書き込み可能に変更
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region 描画先
	// 2. 描画先の変更
	// レンダーターゲートビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region 画面クリア
	// 3. 画面クリア
	FLOAT clearColor[] = { bRed,bGreen,bBule,0.0f };
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

#pragma region ビューポート設定コマンド
	// ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};
	viewport.Width = window_width;   // 横幅
	viewport.Height = window_height; // 縦幅
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
	scissorRect.right = scissorRect.left + window_width;	// 切り抜き座標右
	scissorRect.top = 0;									// 切り抜き座標上
	scissorRect.bottom = scissorRect.top + window_height;	// 切り抜き座標下
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
	CmdFlash();
#pragma endregion
#pragma region コマンド完了待ち
	CmdClear();
#pragma endregion
}

void KDirectXCommon::CmdFlash()
{
	// 命令のクローズ
	result = cmdList->Close();
	assert(SUCCEEDED(result));
	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList };
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	// 画面に表示するバッファをフリップ(裏表の入れ替え)
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));
}

void KDirectXCommon::CmdClear()
{
	// コマンドの完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	// キューをクリア
	result = cmdAllocater->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドを貯める準備
	result = cmdList->Reset(cmdAllocater.Get(), nullptr);
	assert(SUCCEEDED(result));
}

void KDirectXCommon::DXGIFactory() {
	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));
}

void KDirectXCommon::Adapter() {
	// アダプターの列挙用
	std::vector<IDXGIAdapter4*> adapters;

	// ここに特定のアダプターオブジェクトが入る
	IDXGIAdapter4* tmpAdapter = nullptr;

	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++) {
		// 動的配列に追加
		adapters.push_back(tmpAdapter);
	}

	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得
		adapters[i]->GetDesc3(&adapterDesc);

		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

	Device(tmpAdapter);

	tmpAdapter->Release();
}

void KDirectXCommon::Device(IDXGIAdapter4* tmpAdapter) {
	for (size_t i = 0; i < _countof(levels); i++) {
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
}

void KDirectXCommon::CommandList() {
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
}

void KDirectXCommon::CommandQueue() {
	// コマンドキューを生成
	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	assert(SUCCEEDED(result));
}

void KDirectXCommon::SwapChain(KWinApp window) {
	swapChainDesc.Width = window.window_width;
	swapChainDesc.Height = window.window_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapchain1;
	// スワップチェーンの生成
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(), window.hwnd, &swapChainDesc, nullptr, nullptr,
		&swapchain1);

	swapchain1.As(&swapChain);

	assert(SUCCEEDED(result));
}

void KDirectXCommon::Descriptor() {
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
	// デスクリプタヒープの生成
	dev->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
}

void KDirectXCommon::BackBuffer() {
	backBuffers.resize(swapChainDesc.BufferCount);
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
		dev->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
}

void KDirectXCommon::Fence() {
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}