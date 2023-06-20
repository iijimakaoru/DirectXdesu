#pragma once
#include <wrl.h>
#include <d3d12.h>

class KDsvDescriptorHeap
{
private:
	//デバイス
	ID3D12Device* device;

	//デスクプリタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap;

	size_t maxDSV = 2048;

	size_t countDSV = 0;

	UINT incrementSize;

	char PADING2[4];

	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle;

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

