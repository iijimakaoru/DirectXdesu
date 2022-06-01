#pragma once
#include <d3d12.h>
#include "KMaterial.h"
#include "KObject3D.h"
#include "ViewProjection.h"

class KGPlin
{
public:
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
};

