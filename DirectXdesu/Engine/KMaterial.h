#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <vector>

#include "KUtility.h"
#include "KPipelineState.h"
#include "KRootSignature.h"
#include "KShader.h"

using namespace Microsoft::WRL;

using namespace DirectX;

// 定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // 色
};

class KMaterial
{
public:
	KMaterial() = default;
	~KMaterial();
	void Initialize();

private:
	KMaterial(const KMaterial&) = delete;
	KMaterial& operator=(const KMaterial&) = delete;

public:
	// テクスチャデータ
	KTextureData* textureData;

	// 頂点レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;

	// ブレンドステート
	D3D12_BLEND_DESC blenddesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	// パイプラインステート
	std::unique_ptr<KPipelineState> pipelineState;

	// ルートシグネチャ
	std::unique_ptr<KRootSignature> rootSignature;

	// シェーダー
	std::unique_ptr<KShader> shader;

	//深度フラグ
	bool depthFlag = true;
	char PADING[3]{};

	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK;
	char PADING2[4]{};
};

