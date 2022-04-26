#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include <string>
#include"Window.h"

#pragma region using namespace

using namespace std;

#pragma endregion using namespace

class Dx12
{
public:
	Dx12(Window window);
	~Dx12();
	void SetDXGIFactory();
	void SetAdapter();
	void SetDevice(IDXGIAdapter4* tmpAdapter);
	void SetCommandList();
	void SetCommandQueue();
	void SetSwapChain(Window window);
	void SetDescriptor();
	void SetBackBuffer();
	void SetFence();
	
	// 基礎変数
	HRESULT result;
	ID3D12Device* dev = nullptr;
	IDXGIFactory6* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* cmdAllocater = nullptr;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	ID3D12CommandQueue* cmdQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[4] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL featureLevel;

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};


	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// フェンスの生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

	float bRed;
	float bGreen;
	float bBule;
};

