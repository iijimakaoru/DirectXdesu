#pragma once
#include <wrl.h>

struct PipelineSet
{
	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};