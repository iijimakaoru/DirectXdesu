#pragma once
#pragma warning(push)
#pragma warning(disable: 5039)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 4061)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)
#pragma warning(disable: 4514)
#include <d3dx12.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include <chrono>
#pragma warning(pop)

struct  CommandListSet
{
	//アロケーター
	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> allocators;
	//リスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> list;
	//キュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue;
};

class CommandList
{
private:

	//グラフィックス用
	CommandListSet graphic;
	//コンピュート用
	CommandListSet compute;

	const size_t FRAME_BUFFER_COUNT = 2;

	int64_t bbIndex = 0;

public:

	CommandList() = default;
	~CommandList() = default;

	void Initialize(ID3D12Device* device_);

	void ResourceTransition(ID3D12Resource* resource_, uint32_t beforeState_, uint32_t afterState_);

	void GraphicCommandListExecute();
	void ComputeCommandListExecute();

	void GraphicCommandListReset(size_t bbIndex_);
	void ComputeCommandListReset(size_t bbIndex_);

	ID3D12GraphicsCommandList* GetGraphicCommandList();
	ID3D12GraphicsCommandList* GetComputeCommandList();

	ID3D12GraphicsCommandList** GetGraphicCommandListAddressOf();
	ID3D12GraphicsCommandList** GetComputeCommandListAddressOf();

	ID3D12CommandAllocator* GetGraphicCommandAllocator(size_t bbIndex_);
	ID3D12CommandAllocator* GetComputeCommandAllocator(size_t bbIndex_);

	ID3D12CommandQueue* GetComputeCommandQueue();
	ID3D12CommandQueue* GetGraphicCommandQueue();

	void CommandListExecute();
	void BeginCommand(size_t bbIndex_);
	void Close();

private:

	//コピーコンストラクタ・代入演算子削除
	CommandList& operator=(const CommandList&) = delete;
	CommandList(const CommandList&) = delete;
};

