#pragma once
#include <d3d12.h>
#include "KShader.h"
#include "KVertex.h"

class KGPlin
{
public:
	 enum BlendMord
	{
		NONE = 0,
		ADD,
		SUB,
		INV,
		ALPHA,
	};

private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC piplineDesc;

	// パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState;

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

	void SetShader(KShader shader);
	void SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, UINT registerSpace,
		D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, D3D12_DESCRIPTOR_RANGE pDescripterRange, UINT numDescripterRanges,
		D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetRootSignature(UINT rootParamNum = 1);
	void SetScreenRootSignature();
	void Blending(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, const int mord = ALPHA);

public:
	void CreatePipelineAll(KShader shader, bool Obj = false, bool Sprite = false, bool Particle = false, bool Fbx = false);

	void Update(D3D12_PRIMITIVE_TOPOLOGY primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void Setting();
	void SetBlending(int mord);
};

