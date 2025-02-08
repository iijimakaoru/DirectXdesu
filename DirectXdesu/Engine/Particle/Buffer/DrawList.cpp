#include "DrawList.h"
#include "KDirectXCommon.h"

void DrawList::Create(ID3D12DescriptorHeap* uavHeap, uint32_t particleMax)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Device* device = directXCommon->GetDevice();

	UINT64 drawListByteSize = sizeof(ParticleSort) * particleMax;
	UINT64 countBufferOffset = AlignForUavCounter((UINT)drawListByteSize);

	CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC resouceDesc = CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT),
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&resouceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&RWDrawList)
	);
	
	RWDrawList->SetName(L"DrawList");

	D3D12_UNORDERED_ACCESS_VIEW_DESC drawListUAVDescription = {};
	drawListUAVDescription.Format = DXGI_FORMAT_UNKNOWN;
	drawListUAVDescription.Buffer.FirstElement = 0;
	drawListUAVDescription.Buffer.NumElements = particleMax;
	drawListUAVDescription.Buffer.StructureByteStride = sizeof(ParticleSort);
	drawListUAVDescription.Buffer.CounterOffsetInBytes = countBufferOffset;
	drawListUAVDescription.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	drawListUAVDescription.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

	DrawListCPUUAV = CD3DX12_CPU_DESCRIPTOR_HANDLE(uavHeap->GetCPUDescriptorHandleForHeapStart(), 2, directXCommon->GetCBVSRVUAVDescriptorSize());
	DrawListGPUUAV = CD3DX12_GPU_DESCRIPTOR_HANDLE(uavHeap->GetGPUDescriptorHandleForHeapStart(), 2, directXCommon->GetCBVSRVUAVDescriptorSize());
	device->CreateUnorderedAccessView(RWDrawList.Get(), RWDrawList.Get(), &drawListUAVDescription, DrawListCPUUAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC drawListSRVDescription = {};
	drawListSRVDescription.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	drawListSRVDescription.Format = DXGI_FORMAT_UNKNOWN;
	drawListSRVDescription.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	drawListSRVDescription.Buffer.FirstElement = 0;
	drawListSRVDescription.Buffer.NumElements = particleMax;
	drawListSRVDescription.Buffer.StructureByteStride = sizeof(ParticleSort);

	DrawListCPUSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(uavHeap->GetCPUDescriptorHandleForHeapStart(), 5, directXCommon->GetCBVSRVUAVDescriptorSize());
	DrawListGPUSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(uavHeap->GetGPUDescriptorHandleForHeapStart(), 5, directXCommon->GetCBVSRVUAVDescriptorSize());
	device->CreateShaderResourceView(RWDrawList.Get(), &drawListSRVDescription, DrawListCPUSRV);
}

ID3D12Resource* DrawList::GetDrawList()
{
	return RWDrawList.Get();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE& DrawList::GetCPUSRV()
{
	return DrawListCPUSRV;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE& DrawList::GetGPUSRV()
{
	return DrawListGPUSRV;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE& DrawList::GetCPUUAV()
{
	return DrawListCPUUAV;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE& DrawList::GetGPUUAV()
{
	return DrawListGPUUAV;
}

void DrawList::Translation(ID3D12GraphicsCommandList* cmdList, D3D12_RESOURCE_STATES afterState)
{
	CD3DX12_RESOURCE_BARRIER resourceBarrier = 
		CD3DX12_RESOURCE_BARRIER::Transition(RWDrawList.Get(), resourseState, afterState);
	cmdList->ResourceBarrier(1, &resourceBarrier);
	resourseState = afterState;
}
