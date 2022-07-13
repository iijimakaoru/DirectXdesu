#pragma once
#include <d3d12.h>
#include "KMaterial.h"
#include "KWorldTransform.h"
#include "ViewProjection.h"
#include "KMaterial.h"
#include "ViewProjection.h"
#include "KShader.h"
#include "KVertex.h"
#include "KDirectInit.h"

class KGPlin
{
public:
	KGPlin();
	KGPlin(ID3D12Device* dev, int width, int height, KVertex vertex);
	void GPipeline(KVertex vertex);
	void Render();
	void Buffer(ID3D12Device* dev,
		int width, int height);
	void DescRipRan();
	void RootParam();
	void Sampler();
	void RootSig(ID3D12Device* dev);
	void PipelineState(ID3D12Device* dev);

	// シェーダークラス呼び込み
	/*KPixelShader* pxShader;
	KVertexShader* vtShader;*/
	KShader* shader;
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	// マテリアル
	KMaterial* material;
	// 3Dオブジェクト
	KWorldTransform* object3d;
	// ビュープロジェクション
	ViewProjection* viewProjection;
	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParam[3] = {};
	// テクスチャサンブラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	// パイプラインステート
	ID3D12PipelineState* pipelineState = nullptr;

	HRESULT result;
};

