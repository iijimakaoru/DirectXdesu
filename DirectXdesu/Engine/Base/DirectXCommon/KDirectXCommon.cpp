#include "KDirectXCommon.h"
#include "KWinApp.h"
#include <thread>

KDirectXCommon* KDirectXCommon::directXCommon_ = nullptr;

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
	EnbleDebugLayer();
#endif

	//DirectX12関連初期化
	if (FAILED(InitDXGIDevice()))
	{
		assert(0);
		return;
	}

#ifdef _DEBUG
	EnbleInfoQueue();
#endif

	if (FAILED(InitCommand()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreateSwapChain()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreateDepthBuffer()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreateFinalRenderTarget()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreateFence()))
	{
		assert(0);
		return;
	}

	//デスクプリタヒープ生成
	srvHeap = std::make_unique<KDescriptorHeap>();
	srvHeap->Initialize();
}

void KDirectXCommon::PreDraw()
{
	//1バックバッファ番号を取得
	bbIndex = swapChain->GetCurrentBackBufferIndex();
	//書き込み可能に変更

	backBuffers[bbIndex]->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList.Get());

	cmdList->OMSetRenderTargets(1, &backBuffers[bbIndex]->GetHandle(), false, &depthBuff->GetHandle());

	//3画面クリア
	cmdList->ClearRenderTargetView(backBuffers[bbIndex]->GetHandle(), clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(depthBuff->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//ビューポート設定
	viewport.Width = (FLOAT)KWinApp::GetInstance()->GetWindowSizeW();
	viewport.Height = (FLOAT)KWinApp::GetInstance()->GetWindowSizeH();
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// ビューポート設定コマンドを、コマンドリストに積む
	cmdList->RSSetViewports(1, &viewport);

	// シザー矩形設定
	scissorRect.left = 0; // 切り抜き座標左
	scissorRect.right = scissorRect.left + KWinApp::GetInstance()->GetWindowSizeW(); // 切り抜き座標右
	scissorRect.top = 0; // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + KWinApp::GetInstance()->GetWindowSizeH(); // 切り抜き座標下

	// シザー矩形設定コマンドを、コマンドリストに積む
	cmdList->RSSetScissorRects(1, &scissorRect);
}

void KDirectXCommon::PostDraw()
{
	//5元に戻す
	backBuffers[bbIndex]->Transition(D3D12_RESOURCE_STATE_PRESENT, cmdList.Get());

	DeleteCommand();
	// FPS固定
	UpdateFixFPS();
}

void KDirectXCommon::DeleteCommand()
{
	//命令のクローズ
	result = cmdList->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* commandListts[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, commandListts);

	//フリップ
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	//コマンド実行完了を待つ
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

	//キューをクリア
	result = cmdAllocater->Reset();
	assert(SUCCEEDED(result));
	//コマンドリストを貯める準備
	if (cmdList != 0)
	{
		result = cmdList->Reset(cmdAllocater.Get(), nullptr);
		assert(SUCCEEDED(result));
	}
	else
	{
		assert(SUCCEEDED(0));
	}
}

void KDirectXCommon::Destroy()
{
	delete directXCommon_;
}

void KDirectXCommon::SetBackScreenColor(float R, float G, float B, float A)
{
	clearColor[0] = R;
	clearColor[1] = G;
	clearColor[2] = B;
	clearColor[3] = A;
}

ID3D12Device* KDirectXCommon::GetDev() const
{
	return dev.Get();
}

ID3D12GraphicsCommandList* KDirectXCommon::GetCmdlist()
{
	return cmdList.Get();
}

KDescriptorHeap* KDirectXCommon::GetSRVDescriptorHeap()
{
	return srvHeap.get();
}

KRtvDescriptorHeap* KDirectXCommon::GetRTVDescriptorHeap()
{
	return rtvHeap.get();
}

KDsvDescriptorHeap* KDirectXCommon::GetDsvDescriptorHrap()
{
	return dsvHeap.get();
}

void KDirectXCommon::Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource, beforeState, afterState);
	cmdList->ResourceBarrier(1, &barrier);
}

size_t KDirectXCommon::GetBackBufferCount() const
{
	return backBuffers.size();
}

HRESULT KDirectXCommon::InitDXGIDevice()
{
	//DXGIファクトリー
	result = CreateDXGIFactory(IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return result;
	}

	//アダプター列挙用
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter4>> adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	Microsoft::WRL::ComPtr<IDXGIAdapter4> tmpAdapter;

	//パフォーマンスが高いのもから全て列挙
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(tmpAdapter.ReleaseAndGetAddressOf())) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}

	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);

		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

	//Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++) {
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(dev.GetAddressOf()));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	return result;
}

HRESULT KDirectXCommon::CreateFinalRenderTarget()
{
	//デスクリプタヒープの生成
	rtvHeap = std::make_unique<KRtvDescriptorHeap>();
	rtvHeap->Initialize();

	//バックバッファ
	backBuffers.resize(swapChainDesc.BufferCount);

	//スワップチェーンのバッファを処理
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// 生成
		backBuffers[i] = std::make_unique<KRenderTargetBuffer>();
		

		// 生成
		backBuffers[i]->Create(swapChain.Get(), static_cast<UINT>(i));
	}

	return S_OK;
}

HRESULT KDirectXCommon::CreateSwapChain()
{
	swapChainDesc.Width = static_cast<UINT>(KWinApp::GetInstance()->GetWindowSizeW());//横幅
	swapChainDesc.Height = static_cast<UINT>(KWinApp::GetInstance()->GetWindowSizeH());//縦幅
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色情報の書式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルなし
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//バックバッファ用
	swapChainDesc.BufferCount = 2;//バッファ数2
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//生成
	if (cmdQueue != 0)
	{
		Microsoft::WRL::ComPtr<IDXGISwapChain1> tmpSwapChain;
		result = dxgiFactory->CreateSwapChainForHwnd(cmdQueue.Get(), KWinApp::GetInstance()->GetHWND(), &swapChainDesc, nullptr, nullptr, tmpSwapChain.ReleaseAndGetAddressOf());
		tmpSwapChain.As(&swapChain);

		if (FAILED(result))
		{
			return result;
		}
	}
	else
	{
		assert(SUCCEEDED(0));
	}

	return result;
}

HRESULT KDirectXCommon::InitCommand()
{
	//コマンドアロケータを生成
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocater.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return result;
	}

	//コマンドリストを生成
	if (cmdAllocater != 0)
	{
		result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocater.Get(), nullptr, IID_PPV_ARGS(cmdList.ReleaseAndGetAddressOf()));
		if (FAILED(result))
		{
			return result;
		}
	}
	else
	{
		assert(SUCCEEDED(0));
	}

	//コマンドキューの設定＆生成
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	result = dev->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(cmdQueue.ReleaseAndGetAddressOf()));

	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT KDirectXCommon::CreateFence()
{
	//フェンスの生成
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));

	return result;
}

HRESULT KDirectXCommon::CreateDepthBuffer()
{
	//デスクプリタヒープ生成
	dsvHeap = std::make_unique<KDsvDescriptorHeap>();
	dsvHeap->Initialize();

	//深度バッファ生成
	depthBuff = std::make_unique<KDepthStencilBuffer>(dev.Get(), dsvHeap.get());
	depthBuff->Create(static_cast<UINT>(KWinApp::GetInstance()->GetWindowSizeW()),
		static_cast<UINT>(KWinApp::GetInstance()->GetWindowSizeH()),
		DXGI_FORMAT_D32_FLOAT);

	return result;
}

void KDirectXCommon::EnbleDebugLayer()
{
	Microsoft::WRL::ComPtr <ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.ReleaseAndGetAddressOf()))))
	{
		debugController->EnableDebugLayer();
		//debugController->SetEnableGPUBasedValidation(true);
	}
}

void KDirectXCommon::EnbleInfoQueue()
{
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
	result = dev->QueryInterface(IID_PPV_ARGS(&infoQueue));
	if (SUCCEEDED(result))
	{
		// ヤバい
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		// 普通
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		// 警告
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//抑制するエラー
		D3D12_MESSAGE_ID denyIds[] =
		{
			/*
			*windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤー
			*相互作用バグによるエラーメッセージ
			*/
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE

		};
		//抑制する表示レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したエラーの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
	}
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

	float nowTime = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::steady_clock::now() - reference_).count()) / 1000000.0f;

	if (nowTime > 0)
	{
		fps = (fps * 0.99f) + (0.01f / nowTime);
	}
	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

KDirectXCommon* KDirectXCommon::GetInstance()
{
	if (directXCommon_ == nullptr)
	{
		directXCommon_ = new KDirectXCommon();
	}

	return directXCommon_;
}

void KDirectXCommon::ResourceTransition(ID3D12Resource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	GetInstance()->Transition(resource, beforeState, afterState);
}
