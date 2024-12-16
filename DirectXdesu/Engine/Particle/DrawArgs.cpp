#include "DrawArgs.h"
#include "KDirectXCommon.h"

void DrawArgs::Create(ID3D12DescriptorHeap* uavHeap)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Device* device = directXCommon->GetDevice();

	UINT64 drawArgsByteSize = (sizeof(unsigned int) * 9);
	UINT64 countBufferOffset = AlignForUavCounter((UINT)drawArgsByteSize);

	CD3DX12_HEAP_PROPERTIES heap =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC resouceDesc =
		CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT),
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&resouceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&RWDrawArgs)
	);
	RWDrawArgs.Get()->SetName(L"DrawArgs");

	D3D12_UNORDERED_ACCESS_VIEW_DESC drawArgsUAVDescription = {};
	drawArgsUAVDescription.Format = DXGI_FORMAT_UNKNOWN;
	drawArgsUAVDescription.Buffer.FirstElement = 0;
	drawArgsUAVDescription.Buffer.NumElements = 9;
	drawArgsUAVDescription.Buffer.StructureByteStride = sizeof(unsigned int);
	drawArgsUAVDescription.Buffer.CounterOffsetInBytes = countBufferOffset;
	drawArgsUAVDescription.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	drawArgsUAVDescription.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

	DrawArgsCPUUAV =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(uavHeap->GetCPUDescriptorHandleForHeapStart(), 3, directXCommon->GetCBVSRVUAVDescriptorSize());
	DrawArgsGPUUAV =
		CD3DX12_GPU_DESCRIPTOR_HANDLE(uavHeap->GetGPUDescriptorHandleForHeapStart(), 3, directXCommon->GetCBVSRVUAVDescriptorSize());
	device->CreateUnorderedAccessView(RWDrawArgs.Get(), RWDrawArgs.Get(), &drawArgsUAVDescription, DrawArgsCPUUAV);
}

ID3D12Resource* DrawArgs::GetDrawArgs()
{
	return RWDrawArgs.Get();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DrawArgs::GetCPUUAV()
{
	return DrawArgsCPUUAV;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DrawArgs::GetGPUUAV()
{
	return DrawArgsGPUUAV;
}
