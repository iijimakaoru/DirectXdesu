#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include <string>
#include <DirectXTex.h>
#include"KWinApp.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace DirectX;

using namespace Microsoft::WRL;

class KDirectInit
{
public:
	KDirectInit(KWinApp window);
	void CmdFlash();
	void CmdClear();

	ComPtr<ID3D12Device> SetDev() { return dev; }
	ID3D12GraphicsCommandList* SetCmdlist() { return cmdList; }
	ComPtr<IDXGISwapChain4> SetSChain() { return swapChain; }
	ComPtr<ID3D12CommandAllocator> SetCmdAllocater() { return cmdAllocater; }
	ID3D12DescriptorHeap* SetRtvHeap() { return rtvHeap; }
	D3D12_DESCRIPTOR_HEAP_DESC SetRtvHeapDesc() { return rtvHeapDesc; }
	std::vector<ComPtr<ID3D12Resource>> SetBackBuffers() { return backBuffers; }

	float bRed = 0.1f;
	float bGreen = 0.25f;
	float bBule = 0.5f;

private:
	HRESULT result;
	ComPtr<ID3D12Device> dev;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocater;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[4] = {
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

	// バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	// フェンスの生成
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	void DXGIFactory();
	void Adapter();
	void Device(IDXGIAdapter4* tmpAdapter);
	void CommandList();
	void CommandQueue();
	void SwapChain(KWinApp window);
	void Descriptor();
	void BackBuffer();
	void Fence();
};

