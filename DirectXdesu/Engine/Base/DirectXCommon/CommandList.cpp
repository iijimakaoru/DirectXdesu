#include "CommandList.h"

void CommandList::Initialize(ID3D12Device* device_)
{

	HRESULT result;
	D3D12_COMMAND_LIST_TYPE lListType = D3D12_COMMAND_LIST_TYPE_DIRECT;
	//グラフィックス用
	{
		//コマンドアロケーターを生成
		graphic.allocators.resize(FRAME_BUFFER_COUNT);
		for (size_t i = 0; i < FRAME_BUFFER_COUNT; ++i)
		{
			result = device_->CreateCommandAllocator(lListType, IID_PPV_ARGS(&graphic.allocators[i])
			);
			if (FAILED(result))
			{
				printf("Failed CreateCommandAllocator");
			}
		}

		//コマンドキューの設定＆生成
		D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{};
		lCommandQueueDesc.Type = lListType;
		result = device_->CreateCommandQueue(&lCommandQueueDesc, IID_PPV_ARGS(graphic.queue.ReleaseAndGetAddressOf()));
		if (FAILED(result))
		{
			printf("Failed CreateCommandQueue");
		}

		//コマンドリストを生成
		result = device_->CreateCommandList(0, lListType, graphic.allocators[0].Get(), nullptr, IID_PPV_ARGS(graphic.list.ReleaseAndGetAddressOf()));
		if (FAILED(result))
		{
			printf("Failed CreateCommandList");
		}

		graphic.list->Close();
	}

	lListType = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	//コンピュート用
	{
		//コマンドアロケーターを生成
		compute.allocators.resize(FRAME_BUFFER_COUNT);
		for (size_t i = 0; i < FRAME_BUFFER_COUNT; ++i)
		{
			result = device_->CreateCommandAllocator(lListType, IID_PPV_ARGS(&compute.allocators[i])
			);
			if (FAILED(result))
			{
				printf("Failed CreateCommandAllocator");
			}
		}

		//コマンドキューの設定＆生成
		D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{};
		lCommandQueueDesc.Type = lListType;
		result = device_->CreateCommandQueue(&lCommandQueueDesc, IID_PPV_ARGS(compute.queue.ReleaseAndGetAddressOf()));
		if (FAILED(result))
		{
			printf("Failed CreateCommandQueue");
		}

		//コマンドリストを生成
		result = device_->CreateCommandList(0, lListType, compute.allocators[0].Get(), nullptr, IID_PPV_ARGS(compute.list.ReleaseAndGetAddressOf()));
		if (FAILED(result))
		{
			printf("Failed CreateCommandList");
		}

		compute.list->Close();
	}
}

#pragma region グラフィックス

void CommandList::GraphicCommandListExecute()
{
	HRESULT result;

	result = graphic.list->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* lCommandListts[] = { graphic.list.Get() };
	graphic.queue->ExecuteCommandLists(1, lCommandListts);
}

void CommandList::GraphicCommandListReset(size_t bbIndex_)
{
	HRESULT result;

	//キューをクリア
	result = graphic.allocators[bbIndex_]->Reset();
	assert(SUCCEEDED(result));

	//コマンドリストを貯める準備
	result = graphic.list->Reset(graphic.allocators[bbIndex_].Get(), nullptr);
	assert(SUCCEEDED(result));
}

void CommandList::ResourceTransition(ID3D12Resource* resource_, uint32_t beforeState_, uint32_t afterState_)
{
	auto lBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resource_, static_cast<D3D12_RESOURCE_STATES>(beforeState_), static_cast<D3D12_RESOURCE_STATES>(afterState_));
	graphic.list->ResourceBarrier(1, &lBarrier);
}

ID3D12GraphicsCommandList* CommandList::GetGraphicCommandList()
{
	return graphic.list.Get();
}

ID3D12GraphicsCommandList** CommandList::GetGraphicCommandListAddressOf()
{
	return graphic.list.GetAddressOf();
}

ID3D12CommandAllocator* CommandList::GetGraphicCommandAllocator(size_t bbIndex_)
{
	return graphic.allocators[bbIndex_].Get();
}

ID3D12CommandQueue* CommandList::GetGraphicCommandQueue()
{
	return graphic.queue.Get();
}
#pragma endregion

#pragma region コンピュート

ID3D12GraphicsCommandList* CommandList::GetComputeCommandList()
{
	return compute.list.Get();
}

void CommandList::ComputeCommandListExecute()
{
	HRESULT result;

	result = compute.list->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* lCommandListts[] = { compute.list.Get() };
	compute.queue->ExecuteCommandLists(1, lCommandListts);
}

void CommandList::ComputeCommandListReset(size_t bbIndex_)
{
	HRESULT result;

	//キューをクリア
	result = compute.allocators[bbIndex_]->Reset();
	assert(SUCCEEDED(result));

	//コマンドリストを貯める準備
	result = compute.list->Reset(compute.allocators[bbIndex_].Get(), nullptr);
	assert(SUCCEEDED(result));
}

ID3D12GraphicsCommandList** CommandList::GetComputeCommandListAddressOf()
{
	return compute.list.GetAddressOf();
}

ID3D12CommandAllocator* CommandList::GetComputeCommandAllocator(size_t bbIndex_)
{
	return compute.allocators[bbIndex_].Get();
}

ID3D12CommandQueue* CommandList::GetComputeCommandQueue()
{
	return compute.queue.Get();
}

#pragma endregion

void CommandList::CommandListExecute()
{
	GraphicCommandListExecute();
	ComputeCommandListExecute();
}

void CommandList::BeginCommand(size_t bbIndex_)
{
	GraphicCommandListReset(bbIndex_);
	ComputeCommandListReset(bbIndex_);
}