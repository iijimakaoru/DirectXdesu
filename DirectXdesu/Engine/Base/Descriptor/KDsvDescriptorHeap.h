#pragma once
#pragma warning(push)
#pragma warning(disable: 5039)
#pragma warning(disable: 4668)
#pragma warning(disable: 5024)
#pragma warning(disable: 4820)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 5220)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4265)
#pragma warning(disable: 5204)
#include <wrl.h>
#include <d3d12.h>
#pragma warning(pop)

/**
 * @file KDsvDescripterHeap.h
 * @brief 深度ステンシルビューデスクリプタヒープ
 * @author 飯島 薫
 */

namespace MesiEngine {
class KDsvDescriptorHeap {
private:
	// デバイス
	ID3D12Device* device;

	// デスクプリタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;

	size_t maxDSV = 2048;

	size_t countDSV = 0;

	UINT incrementSize;

	char PADING2[4];

	D3D12_CPU_DESCRIPTOR_HANDLE startCpuHandle;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// デプスステンシルビューを生成
	/// </summary>
	/// <param name="desc"> デプスステンシルビューデスク</param>
	/// <param name="resource">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	UINT64 CreateDSV(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, ID3D12Resource* resource);

	/// <summary>
	/// デスクプリタヒープを取得
	/// </summary>
	ID3D12DescriptorHeap* GetHeap();

	UINT GetIncrementSize();

	~KDsvDescriptorHeap() = default;
};
} // namespace MesiEngine
