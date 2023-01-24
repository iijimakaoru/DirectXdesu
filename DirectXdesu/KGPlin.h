#pragma once
#include <d3d12.h>
#include "KShader.h"
#include "KVertex.h"
#include "KDirectXCommon.h"
#include "PipelineSet.h"

class KGPlin
{
public:
	static enum BlendMord
	{
		NONE = 0,
		ADD,
		SUB,
		INV,
		ALPHA,
	};
private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC piplineDesc;
	PipelineSet piplineSet;

	void SetShader(KShader shader);
	void SetRootParam(D3D12_ROOT_PARAMETER& rootParam,
		D3D12_ROOT_PARAMETER_TYPE type,
		UINT shaderRegister,
		UINT registerSpace,
		D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetRootParam(D3D12_ROOT_PARAMETER& rootParam,
		D3D12_ROOT_PARAMETER_TYPE type,
		D3D12_DESCRIPTOR_RANGE pDescripterRange,
		UINT numDescripterRanges,
		D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetRootSignature(UINT rootParamNum = 1);
	void SetScreenRootSignature();
	void Blending(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, const int mord = NONE);

public:
	KGPlin();
	KGPlin(KShader shader);
	KGPlin(D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, KShader shader);
	KGPlin(KShader shader,
		D3D12_INPUT_ELEMENT_DESC* inputLayout,
		UINT inputLayoutSize,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		D3D12_FILL_MODE fillmord = D3D12_FILL_MODE_SOLID);
	void Init(KShader shader,
		D3D12_INPUT_ELEMENT_DESC* inputLayout,
		UINT inputLayoutSize,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		D3D12_FILL_MODE fillmord = D3D12_FILL_MODE_SOLID,
		D3D12_CULL_MODE cullmord = D3D12_CULL_MODE_BACK,
		bool isDeep = true);
	void Update(D3D12_PRIMITIVE_TOPOLOGY primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void Setting();
};

