#pragma once
struct PipelineSet
{
	// パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
};