#include "KComputePipelineState.h"
#include <assert.h>

void KComputePipelineState::SetShader(D3D12_SHADER_BYTECODE& shader)
{
	shaderByte = shader;
}

void KComputePipelineState::SetRootSignature(KRootSignature* rootSignature_)
{
	rootSignaturePtr = rootSignature_;
}

void KComputePipelineState::SetNodeMask(UINT nodeMask)
{
	pipelineNodeMask = nodeMask;
}

void KComputePipelineState::SetCachedPSO(D3D12_CACHED_PIPELINE_STATE& cachedPSO)
{
	cachedPipeline = cachedPSO;
}

void KComputePipelineState::SetFlag(D3D12_PIPELINE_STATE_FLAGS flag)
{
	pipelineFlag = flag;
}

void KComputePipelineState::Create(ID3D12Device* device)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};
	pipelineDesc.CS = shaderByte;
	pipelineDesc.pRootSignature = rootSignaturePtr->GetRootSignature();
	pipelineDesc.NodeMask = pipelineNodeMask;
	pipelineDesc.CachedPSO = cachedPipeline;
	pipelineDesc.Flags = pipelineFlag;

	HRESULT result = device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(pipelineState.GetAddressOf()));

	if (FAILED(result))
	{
		assert(0);
	}
}

ID3D12PipelineState* KComputePipelineState::GetPipelineState()
{
	return pipelineState.Get();
}
