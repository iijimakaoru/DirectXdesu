#pragma once

#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 4514)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)
#include <dxgi1_6.h>
#include <roapi.h>
#include <winioctl.h>
#include <intsafe.h>
#include <WinBase.h>
#pragma warning(pop)
#include "Vector4.h"
#include "KRtvDescriptorHeap.h"

class KRenderTargetBuffer
{
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
	/// リソースを取得
	/// </summary>
	ID3D12Resource* GetTexture() const;

	/// <summary>
	/// ポインタ番号を取得
	/// </summary>
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetHandle();

	void Transition(D3D12_RESOURCE_STATES resourceStates, ID3D12GraphicsCommandList* commandList);

	KRenderTargetBuffer(ID3D12Device* device_, KRtvDescriptorHeap* rtvHeap_);

	~KRenderTargetBuffer() = default;

private:
	//ステータス
	D3D12_RESOURCE_STATES states;
	//幅
	UINT width = 0;
	//高さ
	UINT height = 0;
	//テクスチャ
	Microsoft::WRL::ComPtr<ID3D12Resource> texture;
	//ディスクリプタヒープ
	KRtvDescriptorHeap* rtvHeap = nullptr;
	//デバイス
	ID3D12Device* device = nullptr;
	//ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handle{};
};

