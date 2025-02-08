#include "ParticlePool.h"
#include "KDirectXCommon.h"

void ParticlePool::Create(ID3D12DescriptorHeap* uavHeap, uint32_t particleMax)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Device* device = directXCommon->GetDevice();

	UINT64 particlePoolByteSize =
		sizeof(Particle) * particleMax;
	CD3DX12_HEAP_PROPERTIES heap =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC resouceDesc = CD3DX12_RESOURCE_DESC::Buffer(particlePoolByteSize,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&resouceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&RWParticlePool));
	resourseState = D3D12_RESOURCE_STATE_COMMON;

	Translation(directXCommon->GetCommandList(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	RWParticlePool->SetName(L"ParticlePool");

	D3D12_UNORDERED_ACCESS_VIEW_DESC particlePoolUAVDescription = {};
	particlePoolUAVDescription.Format = DXGI_FORMAT_UNKNOWN;
	particlePoolUAVDescription.Buffer.FirstElement = 0;
	particlePoolUAVDescription.Buffer.NumElements = particleMax;
	particlePoolUAVDescription.Buffer.StructureByteStride = sizeof(Particle);
	particlePoolUAVDescription.Buffer.CounterOffsetInBytes = 0;
	particlePoolUAVDescription.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

	D3D12_SHADER_RESOURCE_VIEW_DESC particlePoolSRVDescription = {};
	particlePoolSRVDescription.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	particlePoolSRVDescription.Format = DXGI_FORMAT_UNKNOWN;
	particlePoolSRVDescription.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	particlePoolSRVDescription.Buffer.FirstElement = 0;
	particlePoolSRVDescription.Buffer.NumElements = particleMax;
	particlePoolSRVDescription.Buffer.StructureByteStride = sizeof(Particle);

	ParticlePoolCPUUAV =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(uavHeap->GetCPUDescriptorHandleForHeapStart(), 0, directXCommon->GetCBVSRVUAVDescriptorSize());
	ParticlePoolGPUUAV =
		CD3DX12_GPU_DESCRIPTOR_HANDLE(uavHeap->GetGPUDescriptorHandleForHeapStart(), 0, directXCommon->GetCBVSRVUAVDescriptorSize());
	device->CreateUnorderedAccessView(RWParticlePool.Get(), nullptr, &particlePoolUAVDescription, ParticlePoolCPUUAV);

	ParticlePoolCPUSRV =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(uavHeap->GetCPUDescriptorHandleForHeapStart(), 4, directXCommon->GetCBVSRVUAVDescriptorSize());
	ParticlePoolGPUSRV =
		CD3DX12_GPU_DESCRIPTOR_HANDLE(uavHeap->GetGPUDescriptorHandleForHeapStart(), 4, directXCommon->GetCBVSRVUAVDescriptorSize());
	device->CreateShaderResourceView(RWParticlePool.Get(), &particlePoolSRVDescription, ParticlePoolCPUSRV);
}

ID3D12Resource* ParticlePool::GetParticlePool()
{
	return RWParticlePool.Get();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE& ParticlePool::GetCPUSRV()
{
	return ParticlePoolCPUSRV;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE& ParticlePool::GetGPUSRV()
{
	return ParticlePoolGPUSRV;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE& ParticlePool::GetCPUUAV()
{
	return ParticlePoolCPUUAV;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE& ParticlePool::GetGPUUAV()
{
	return ParticlePoolGPUUAV;
}

void ParticlePool::Translation(ID3D12GraphicsCommandList* cmdList, D3D12_RESOURCE_STATES afterState)
{
	CD3DX12_RESOURCE_BARRIER resourceBarrier =
		CD3DX12_RESOURCE_BARRIER::Transition(RWParticlePool.Get(), resourseState, afterState);
	cmdList->ResourceBarrier(1, &resourceBarrier);
	resourseState = afterState;
}
