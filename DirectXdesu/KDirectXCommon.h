#pragma once
#include <d3dx12.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include"KWinApp.h"
#include "KWinApp.h"
#include "KDepth.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

class KDirectXCommon
{
public:
	KDirectXCommon();
	~KDirectXCommon()
	{
		
	}
	void Init(KWinApp* win);

	void InitDev();

	void InitCommand();

	void InitSwapChain();

	void InitRenderTargetView();

	void InitDepthBuffer();

	void InitFence();

	void PreDraw();

	void PostDraw();

	void CmdFlash();

	void CmdClear();

	ComPtr<ID3D12Device> SetDev() { return dev; }
	ComPtr<ID3D12GraphicsCommandList> SetCmdlist() { return cmdList; }
	ComPtr<IDXGISwapChain4> SetSChain() { return swapChain; }
	ComPtr<ID3D12CommandAllocator> SetCmdAllocater() { return cmdAllocater; }
	ComPtr<ID3D12DescriptorHeap> SetRtvHeap() { return rtvHeap; }
	//D3D12_DESCRIPTOR_HEAP_DESC SetRtvHeapDesc() { return rtvHeapDesc; }
	std::vector<ComPtr<ID3D12Resource>> SetBackBuffers() { return backBuffers; }

private:
	HRESULT result;
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
	// DXGIファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
	// バックバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	// 深度バッファ
	ComPtr<ID3D12Resource> depthBuff{};
	//　深度ビュー用ヒープ作成
	ComPtr<ID3D12DescriptorHeap> dsvHeap{};
	ComPtr<ID3D12DescriptorHeap> tmpDsvHeap{};

	ComPtr<ID3D12CommandAllocator> cmdAllocater;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	D3D_FEATURE_LEVEL featureLevel;

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// フェンスの生成
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	// 1.リソースバリアで書き込み可能に変更
	D3D12_RESOURCE_BARRIER barrierDesc{};

	float bRed = 0.1f;
	float bGreen = 0.25f;
	float bBule = 0.5f;

	KWinApp* win = nullptr;

	//KDepth* depth = nullptr;
};

