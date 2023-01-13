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

using namespace Microsoft::WRL;

class KDirectXCommon
{
public:
	void Init();

	void PreDraw();

	void PostDraw();

	void CmdFlash();

	void CmdClear();

	// デバイス取得
	ID3D12Device* GetDev() const { return dev.Get(); }
	ID3D12GraphicsCommandList* GetCmdlist() { return cmdList.Get(); }

	float fps = 0;

private:
	HRESULT result;
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
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

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	ComPtr<ID3D12CommandAllocator> cmdAllocater;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	// フェンスの生成
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	// 1.リソースバリアで書き込み可能に変更
	D3D12_RESOURCE_BARRIER barrierDesc{};

	float bRed = 0.1f;
	float bGreen = 0.25f;
	float bBule = 0.5f;

	//KDepth* depth = nullptr;

	std::chrono::steady_clock::time_point reference_;

	// FPS固定初期化
	void InitFixFPS();
	// FPS固定
	void UpdateFixFPS();

public:
	static KDirectXCommon* GetInstance();

private:
	KDirectXCommon() = default;
	~KDirectXCommon() = default;
	KDirectXCommon(const KDirectXCommon&) = default;
	const KDirectXCommon& operator=(const KDirectXCommon&) = delete;
};

