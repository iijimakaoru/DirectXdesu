#pragma once
#include <wrl.h>
#include <d3d12.h>

#include "KShader.h"
#include "RootSignature.h"

class ComputePipelineState
{
private:
	KShader csShader;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> PSOs;

	RootSignature* rootSignature_ = nullptr;

	//フラグ
	D3D12_PIPELINE_STATE_FLAGS  pipelineFlag = D3D12_PIPELINE_STATE_FLAG_NONE;

public:
	void CreateShader(LPCWSTR fileName, LPCSTR entryPoint);

	void SetRootSignature(RootSignature* rootSignature);

	/// <summary>
	/// フラグをセット
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetFlag(D3D12_PIPELINE_STATE_FLAGS flag_);

	/// <summary>
	/// 生成
	/// </summary>
	void Create(ID3D12Device* device_);

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	/// <returns>パイプラインステート</returns>
	ID3D12PipelineState* GetPipelineState()const;
};

