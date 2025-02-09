#include "CreateBlend.h"

D3D12_BLEND_DESC CreateBlend(BlendMode mode)
{
    D3D12_BLEND_DESC blend{};

	//	共通設定
	if (mode != NONE) {
		blend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		blend.RenderTarget[0].BlendEnable = true;
		blend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		blend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	}
	else {
		blend.AlphaToCoverageEnable = false;
	}

	switch (mode)
	{
	case ADD:
		blend.RenderTarget->BlendOp = D3D12_BLEND_OP_ADD;
		blend.RenderTarget->SrcBlend = D3D12_BLEND_ONE;
		blend.RenderTarget->DestBlend = D3D12_BLEND_ONE;
		break;
	case SUB:
		blend.RenderTarget->BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blend.RenderTarget->SrcBlend = D3D12_BLEND_ONE;
		blend.RenderTarget->DestBlend = D3D12_BLEND_ONE;
		break;
	case INV:
		blend.RenderTarget->BlendOp = D3D12_BLEND_OP_ADD;
		blend.RenderTarget->SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blend.RenderTarget->DestBlend = D3D12_BLEND_ZERO;
		break;
	case ALPHA:
		blend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blend.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	default:
		break;
	}

    return blend;
}
