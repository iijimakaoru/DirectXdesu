#pragma once
#include <wrl.h>
#include <d3d12.h>

class ComputePipelineState
{
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> PSOs;
};

