#pragma once
#include <d3dx12.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include "KDepth.h"
#include <chrono>

#include "KRtvDescriptorHeap.h"
#include "KRenderTargetBuffer.h"
#include "KDescriptorHeap.h"
#include "KDepthStencilBuffer.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class KDirectXCommon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	HRESULT result;
	char PADING[4];

	// DirectX12デバイス
	ComPtr<ID3D12Device> dev;

	// DXGI
	ComPtr<IDXGIFactory6> dxgiFactory;

	// スワップチェーン
	ComPtr<IDXGISwapChain4> swapChain;

	// コマンド
	ComPtr<ID3D12CommandAllocator> cmdAllocater;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;

	// フェンスの生成
	ComPtr<ID3D12Fence> fence;

	// RTV用のデスクリプタヒープ
	std::unique_ptr<KRtvDescriptorHeap> rtvHeap;

	// SRV用のデスクリプタヒープ
	std::unique_ptr<KDescriptorHeap> srvHeap;

	// 深度バッファ
	std::unique_ptr<KDepthStencilBuffer> depthBuff{};

	// DSV用のデスクリプタヒープ
	std::unique_ptr<KDsvDescriptorHeap> dsvHeap{};

	// バックバッファ
	std::vector<std::unique_ptr<KRenderTargetBuffer>> backBuffers;

	// フェンスの生成
	UINT64 fenceVal = 0;

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// クリアカラー
	FLOAT clearColor[4] = { 0.1f,0.25f,0.5f,0.0f };

	//ビューポート
	D3D12_VIEWPORT viewport{};

	//シザー矩形
	D3D12_RECT scissorRect{};

	UINT bbIndex;

	char PADING2[4]{};

	static KDirectXCommon* directXCommon_;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	ComPtr<ID3D12DescriptorHeap> tmpDsvHeap{};

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	D3D_FEATURE_LEVEL featureLevel;

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	// 1.リソースバリアで書き込み可能に変更
	D3D12_RESOURCE_BARRIER barrierDesc{};

	std::chrono::steady_clock::time_point reference_;

	// FPS固定初期化
	void InitFixFPS();
	// FPS固定
	void UpdateFixFPS();

public:
	// シングルトンインスタンス
	static KDirectXCommon* GetInstance();

	// リソースの状態を変える
	static void ResourceTransition(ID3D12Resource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

	// 初期化
	void Init();

	// 描画準備
	void PreDraw();

	// 描画終了
	void PostDraw();

	// コマンド後始末
	void DeleteCommand();

	// 解放
	void Destroy();

	// 背景の色変更
	void SetBackScreenColor(float R, float G, float B, float A);

	// デバイス取得
	ID3D12Device* GetDev() const { return dev.Get(); }

	// コマンドリスト取得
	ID3D12GraphicsCommandList* GetCmdlist() { return cmdList.Get(); }

	float fps = 0;

	// SRV,CBV,URV用のデスクリプタヒープ取得
	KDescriptorHeap* GetSRVDescriptorHeap();

	// RTV用のデスクリプタヒープ取得
	KRtvDescriptorHeap* GetRTVDescriptorHeap();

	// DSV用のデスクリプタヒープ取得
	KDsvDescriptorHeap* GetDsvDescriptorHrap();

	// リソースの状態を変える
	void Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

	// バックバッファの数を取得
	size_t GetBackBufferCount() const;

private:
	// DXGI初期化
	HRESULT InitDXGIDevice();

	// 最終的なレンダーターゲットの生成
	HRESULT CreateFinalRenderTarget();

	// スワップチェーンの生成
	HRESULT CreateSwapChain();

	// コマンド初期化
	HRESULT InitCommand();

	// フェンス生成
	HRESULT CreateFence();

	// 深度関係生成
	HRESULT CreateDepthBuffer();

	// デバッグレイヤーを有効化
	void EnbleDebugLayer();

	// インフォキューを有効化
	void EnbleInfoQueue();

	KDirectXCommon() = default;
	~KDirectXCommon() = default;
	KDirectXCommon(const KDirectXCommon&) = default;
	const KDirectXCommon& operator=(const KDirectXCommon&) = delete;
};

