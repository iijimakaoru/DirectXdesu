#pragma once
#include <dxgi1_6.h>
#include "KRtvDescriptorHeap.h"
#include "Vector4.h"

class KRenderTargetBuffer
{
	//テクスチャ
	Microsoft::WRL::ComPtr<ID3D12Resource> texture;
	//ディスクリプタヒープ
	KRtvDescriptorHeap* rtvHeap = nullptr;
	//幅
	UINT width = 0;
	//高さ
	UINT height = 0;
	//デバイス
	ID3D12Device* device = nullptr;
	//ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handle{};
	//ステータス
	D3D12_RESOURCE_STATES states;
public:

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="w">横幅</param>
	/// <param name="h">縦幅</param>
	/// <param name="resourceStates">ステータス</param>
	/// <param name="mipLevel">ミップレベル</param>
	/// <param name="arraySize">配列のサイズ</param>
	/// <param name="format">フォーマット</param>
	/// <param name="clearColor">カラー</param>
	bool Create(UINT w,
		UINT h,
		D3D12_RESOURCE_STATES resourceStates,
		UINT16 mipLevel = 0,
		UINT16 arraySize = 1,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		const KMyMath::Vector4& clearColor = { 1.0f,1.0f,1.0f,1.0f });

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="swapChain4"></param>
	/// <returns></returns>
	bool Create(IDXGISwapChain4* swapChain, UINT index);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="d3dDevice">デバイス</param>
	/// <param name="descriptorHeap">レンダーターゲット用のデスクプリタヒープ</param>
	KRenderTargetBuffer(ID3D12Device* d3dDevice, KRtvDescriptorHeap* descriptorHeap);

	/// <summary>
	/// リソースを取得
	/// </summary>
	ID3D12Resource* GetTexture() const;

	/// <summary>
	/// ポインタ番号を取得
	/// </summary>
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetHandle();

	void Transition(D3D12_RESOURCE_STATES resourceStates, ID3D12GraphicsCommandList* commandList);

	~KRenderTargetBuffer() = default;

private:

	KRenderTargetBuffer() = delete;

};

