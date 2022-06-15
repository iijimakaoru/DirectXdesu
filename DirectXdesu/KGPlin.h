#pragma once
#include <d3d12.h>
#include "KMaterial.h"
#include "KObject3D.h"
#include "ViewProjection.h"
#include "KMaterial.h"
#include "ViewProjection.h"
#include "KShader.h"
#include "KVertex.h"
#include "KDirectInit.h"

// 頂点レイアウト
static D3D12_INPUT_ELEMENT_DESC inputLayout[3] = {
	{// xyz座標
		"POSITION",										// セマンティック名
		0,												// 同じセマンティック名が複数あるときに使うインデックス
		DXGI_FORMAT_R32G32B32_FLOAT,					// 要素数とビット数を表す
		0,												// 入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,					// データのオフセット
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// 入力データ種別
		0												// 一度に描画するインスタンス数
	},
	{// 法線ベクトル
		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	{// uv座標
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
};

class KGPlin
{
public:
	KGPlin();
	KGPlin(KDirectInit dx,HRESULT result, ID3D12Device* dev,
		int width, int height);
	void GPipeline();
	void Render();
	void Buffer(HRESULT result, ID3D12Device* dev,
		int width, int height);
	void DescRipRan();
	void RootParam();
	void Sampler();
	void RootSig(HRESULT result, ID3D12Device* dev);
	void PipelineState(HRESULT result, ID3D12Device* dev);

	// シェーダー
	Shader* shader;
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	// マテリアル
	KMaterial* material;
	// 3Dオブジェクト
	Object3D* object3d;
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
};

