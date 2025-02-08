#include "DeadList.h"
#include "KDirectXCommon.h"

void DeadList::Create(ID3D12DescriptorHeap* uavHeap, uint32_t particleMax)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Device* device = directXCommon->GetDevice();

	UINT64 deadListByteSize = sizeof(unsigned int) * particleMax;
	UINT64 countBufferOffset = AlignForUavCounter((UINT)deadListByteSize);

	CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC resouceDesc = CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT),
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&resouceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&ACDeadList)
	);
	ACDeadList->SetName(L"ACDeadList");

	D3D12_UNORDERED_ACCESS_VIEW_DESC deadListUAVDescription = {};
	deadListUAVDescription.Format = DXGI_FORMAT_UNKNOWN;
	deadListUAVDescription.Buffer.FirstElement = 0;
	deadListUAVDescription.Buffer.NumElements = particleMax;
	deadListUAVDescription.Buffer.StructureByteStride = sizeof(unsigned	int);
	deadListUAVDescription.Buffer.CounterOffsetInBytes = countBufferOffset;
	deadListUAVDescription.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

	ACDeadListCPUUAV =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(uavHeap->GetCPUDescriptorHandleForHeapStart(), 1, directXCommon->GetCBVSRVUAVDescriptorSize());
	ACDeadListGPUUAV =
		CD3DX12_GPU_DESCRIPTOR_HANDLE(uavHeap->GetGPUDescriptorHandleForHeapStart(), 1, directXCommon->GetCBVSRVUAVDescriptorSize());
	device->CreateUnorderedAccessView(ACDeadList.Get(), ACDeadList.Get(), &deadListUAVDescription, ACDeadListCPUUAV);
}

ID3D12Resource* DeadList::GetDeadList()
{
	return ACDeadList.Get();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DeadList::GetCPUUAV()
{
	return ACDeadListCPUUAV;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DeadList::GetGPUUAV()
{
	return ACDeadListGPUUAV;
}
