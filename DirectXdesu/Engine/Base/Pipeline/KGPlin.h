#pragma once
#include "KShader.h"
#include "KVertex.h"

/**
 * @file KGPlin.h
 * @brief グラフィックスパイプライン
 * @author 飯島 薫
 */

class KGPlin {
public:
	enum BlendMord {
		NONE = 0,
		ADD,
		SUB,
		INV,
		ALPHA,
	};

private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC piplineDesc;

	// パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState;

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

	/// <summary>
	/// シェーダーセット
	/// </summary>
	/// <param name="shader"></param>
	void SetShader(KShader shader);

	/// <summary>
	/// ルートパラメータセット
	/// </summary>
	/// <param name="rootParam"></param>
	/// <param name="type"></param>
	/// <param name="shaderRegister"></param>
	/// <param name="registerSpace"></param>
	/// <param name="shaderVisibility"></param>
	void SetRootParam(
	    D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister,
	    UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetRootParam(
	    D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type,
	    D3D12_DESCRIPTOR_RANGE pDescripterRange, UINT numDescripterRanges,
	    D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

	/// <summary>
	/// ルートシグネチャセット
	/// </summary>
	/// <param name="rootParamNum"></param>
	void SetRootSignature(UINT rootParamNum = 1);
	void SetScreenRootSignature();

	/// <summary>
	/// ブレンドモードセット
	/// </summary>
	/// <param name="blenddesc"></param>
	/// <param name="mord"></param>
	void Blending(D3D12_BLEND_DESC& blenddesc, const int mord = ALPHA);

public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="shaderName"></param>
	/// <returns></returns>
	static KGPlin* Create(KShader shader, std::string shaderName = "Null");

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="primitive"></param>
	void Update(D3D12_PRIMITIVE_TOPOLOGY primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/// <summary>
	/// 描画設定
	/// </summary>
	void Setting();

private:
	/// <summary>
	/// パイプライン生成
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="shaderName"></param>
	void CreatePipelineAll(KShader shader, std::string shaderName = "Null");
};
