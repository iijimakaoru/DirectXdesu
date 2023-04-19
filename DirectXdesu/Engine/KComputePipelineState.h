#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "KRootSignature.h"

class KComputePipelineState
{
private:
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

	//シェーダーオブジェクト
	D3D12_SHADER_BYTECODE shaderByte;

	//ルートシグネチャ
	KRootSignature* rootSignaturePtr;

	//ノードマスク
	UINT pipelineNodeMask = 0;
	char PADING1[4];

	//キャッシュパイプライン
	D3D12_CACHED_PIPELINE_STATE cachedPipeline;

	//フラグ
	D3D12_PIPELINE_STATE_FLAGS  pipelineFlag = D3D12_PIPELINE_STATE_FLAG_NONE;
	char PADING2[4];

public:

public:

	/// <summary>
	/// シェーダーをセット
	/// </summary>
	/// <param name="shader">シェーダー</param>
	void SetShader(D3D12_SHADER_BYTECODE& shader);

	/// <summary>
	/// ルートシグネチャをセット
	/// </summary>
	/// <param name="rootSignature_">ルートシグネチャ</param>
	void SetRootSignature(KRootSignature* rootSignature_);

	/// <summary>
	/// ノードマスクをセット
	/// </summary>
	/// <param name="nodeMask">ノードマスク</param>
	void SetNodeMask(UINT nodeMask);

	/// <summary>
	/// キャッシュパイプラインをセット
	/// </summary>
	/// <param name="cachedPSO">キャッシュパイプライン</param>
	void SetCachedPSO(D3D12_CACHED_PIPELINE_STATE& cachedPSO);

	/// <summary>
	/// フラグをセット
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetFlag(D3D12_PIPELINE_STATE_FLAGS flag);

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="device">デバイス</param>
	void Create(ID3D12Device* device);

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	/// <returns>パイプラインステート</returns>
	ID3D12PipelineState* GetPipelineState();
private:
};

