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
#include <chrono>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class KDirectXCommon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	void Init();

	void PreDraw();

	void PostDraw();

	void CmdFlash();

	void CmdClear();

	// デバイス取得
	ID3D12Device* GetDev() const { return dev.Get(); }
	ID3D12GraphicsCommandList* GetCmdlist() { return cmdList.Get(); }
	int GetBackBufferSize() { return backBuffers.size(); }

	float fps = 0;

private:
	HRESULT result;

	// DirectX12デバイス
	ComPtr<ID3D12Device> dev;

	// バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	// スワップチェーン
	ComPtr<IDXGISwapChain4> swapChain;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// 深度バッファ
	ComPtr<ID3D12Resource> depthBuff{};

	//　深度ビュー用ヒープ作成
	ComPtr<ID3D12DescriptorHeap> dsvHeap{};
	ComPtr<ID3D12DescriptorHeap> tmpDsvHeap{};

	// コマンド
	ComPtr<ID3D12CommandAllocator> cmdAllocater;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

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

	std::chrono::steady_clock::time_point reference_;
	
	// デバイス初期化
	void InitDev();
	// コマンド初期化
	void InitCommand();
	// スワップチェーン初期化
	void InitSwapChain();
	// レンダーターゲットビュー初期化
	void InitRenderTargetView();
	// 深度バッファ初期化
	void InitDepthBuffer();
	// フェンス初期化
	void InitFence();
	// FPS固定初期化
	void InitFixFPS();
	// FPS固定
	void UpdateFixFPS();

public:
	static KDirectXCommon* GetInstance();
	static void DeleteInstance();

private:
	KDirectXCommon() = default;
	~KDirectXCommon() = default;
	KDirectXCommon(const KDirectXCommon&) = default;
	const KDirectXCommon& operator=(const KDirectXCommon&) = delete;
};

