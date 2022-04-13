#include <Windows.h>
#include <stdio.h>
#include<vector>
#include<string>
#include "Window.h"
#include "Std.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#ifdef DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ウィンドウが破棄されたとき
	if (msg == WM_DESTROY)
	{
		// osに対してアプリケーション終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// 変数
#pragma region ウィンドウ
	// ウィンドウサイズ
	const int window_width = 1280;
	const int window_height = 720;

	// ウィンドウクラスの設定
	WNDCLASSEX window{};
	window.cbSize = sizeof(WNDCLASSEX);
	window.lpfnWndProc = (WNDPROC)WindowProc;
	window.lpszClassName = L"DirectX12";
	window.lpszMenuName = L"DirectX12";
	window.hInstance = GetModuleHandle(nullptr);
	window.hCursor = LoadCursor(NULL, IDC_ARROW);

	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&window);
	// ウィンドウサイズ
	RECT rect = { 0,0,window_width,window_height };
	// 自分でサイズを調整する
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	// ウィンドウ作成
	HWND handle = CreateWindow(
		window.lpszClassName,
		window.lpszMenuName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		nullptr,
		nullptr,
		window.hInstance,
		nullptr);
	// ウィンドウを表示状態にする
	ShowWindow(handle, SW_SHOW);
#pragma endregion

	// メッセージ格納用構造体
	MSG msg{};

	//Window* win = new Window();
	HRESULT result;
	ID3D12Device* dev = nullptr;
	IDXGIFactory6* dxgiFactory = nullptr;
	IDXGISwapChain4* swapchain = nullptr;
	ID3D12CommandAllocator* cmdAllocater = nullptr;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

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
	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue));
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
		commandQueue, handle, &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapchain);
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
		swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle.ptr += i * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// レンダーターゲートビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// シェーダーの計算結果をSKGBに変換して書き込む
		// レンダーターゲートビューの生成
	}
#pragma endregion



	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
	{
		// 動的配列に追加
		adapters.push_back(tmpAdapter);
	}

	// ウィンドウ表示
	// ゲームループ
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}
	}

	// ウィンドウクラスを登録解除
	UnregisterClass(window.lpszClassName, window.hInstance);
	//ShowWindow(win->Get(), SW_SHOW);

	//while (msg.message != WM_QUIT)
	//{
	//	// 呼び出し側スレッドが所有しているウィンドウに送信されたメッセージの保留されている物を取得
	//	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	//	{
	//		// 仮想キーメッセージを文字メッセージに変換
	//		TranslateMessage(&msg);
	//		// 1つのウィンドウプロシージャにメッセージを送出する
	//		DispatchMessage(&msg);
	//	}
	//}

	//// ウィンドウクラスの解放
	//delete win;

	return 0;
}