#pragma once
#include <d3d12.h>
#include <wrl.h>

class KRtvDescriptorHeap
{
private:
	//デバイス
	ID3D12Device* device;
	//デスクプリタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap;

	size_t maxRTV = 2048;

	size_t countRTV = 0;

	UINT incrementSize;

	char PADING2[4];


	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// レンダーターゲットビュー生成
	/// </summary>
	/// <param name="desc">レンダーターゲットビューデスク</param>
	/// <param name="resource">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	UINT64 CreateRTV(D3D12_RENDER_TARGET_VIEW_DESC& desc, ID3D12Resource* resource);

	/// <summary>
	/// デスクプリタヒープを取得
	/// </summary>
	ID3D12DescriptorHeap* GetHeap();

	UINT GetIncrementSize();
};

