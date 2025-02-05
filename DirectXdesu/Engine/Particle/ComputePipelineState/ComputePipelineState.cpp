#include "ComputePipelineState.h"
#include <string>

void ComputePipelineState::CreateShader(LPCWSTR fileName, LPCSTR entryPoint)
{
	std::wstring ShaderHeadFileName = L"Resources/Shader/";
	ShaderHeadFileName += fileName;
	csShader.Create(ShaderHeadFileName.c_str(), entryPoint, "cs_5_0");
}

void ComputePipelineState::SetRootSignature(RootSignature* rootSignature)
{
	rootSignature_ = rootSignature;
}

void ComputePipelineState::SetFlag(D3D12_PIPELINE_STATE_FLAGS flag_)
{
	pipelineFlag = flag_;
}

void ComputePipelineState::Create(ID3D12Device* device_)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.pRootSignature = rootSignature_->GetRootSignature();
	pipelineDesc.CS = csShader.GetBytecode();
	pipelineDesc.Flags = pipelineFlag;
	device_->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(&PSOs));
}

ID3D12PipelineState* ComputePipelineState::GetPipelineState() const
{
	return PSOs.Get();
}
