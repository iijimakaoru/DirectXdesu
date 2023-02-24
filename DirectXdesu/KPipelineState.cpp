#include "KPipelineState.h"
#include "KDirectXCommon.h"

void KPipelineState::SetInputLayout(D3D12_INPUT_ELEMENT_DESC* desc, size_t length)
{
	inputLayoutData = desc;
	inputLayoutLength = length;
}

void KPipelineState::SetRootSignature(ID3D12RootSignature* pRootSignature)
{
	rootSignature = pRootSignature;
}

void KPipelineState::SetBlend(D3D12_BLEND_DESC& desc)
{
	blendDesc = desc;
}

void KPipelineState::SetDepthFlag(bool flag)
{
	depthFlag = flag;
}

void KPipelineState::SetVertexShader(D3D12_SHADER_BYTECODE* pVsByte)
{
	vsByte = pVsByte;
}

void KPipelineState::SetPixelShader(D3D12_SHADER_BYTECODE* pPsByte)
{
	psByte = pPsByte;
}

void KPipelineState::SetGeometryShader(D3D12_SHADER_BYTECODE* pGsByte)
{
	gsByte = pGsByte;
}

void KPipelineState::SetHullShader(D3D12_SHADER_BYTECODE* pHsByte)
{
	hsByte = pHsByte;
}

void KPipelineState::SetDomainShader(D3D12_SHADER_BYTECODE* pDsByte)
{
	dsByte = pDsByte;
}

void KPipelineState::SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type)
{
	primitiveType = type;
}

void KPipelineState::SetCullMode(D3D12_CULL_MODE model)
{
	cullMode = model;
}

bool KPipelineState::Create()
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> device = KDirectXCommon::GetInstance()->GetDev();

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	if (vsByte)
	{
		pipelineDesc.VS = *vsByte;
	}
	if (psByte)
	{
		pipelineDesc.PS = *psByte;
	}
	if (dsByte)
	{
		pipelineDesc.DS = *dsByte;
	}
	if (hsByte)
	{
		pipelineDesc.HS = *hsByte;
	}
	if (gsByte)
	{
		pipelineDesc.GS = *gsByte;
	}

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = cullMode; // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	//ブレンドデスク
	pipelineDesc.BlendState = blendDesc;

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputLayoutLength);
	pipelineDesc.InputLayout.pInputElementDescs = inputLayoutData;

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = primitiveType;
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = depthFlag;//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度フォーマット
	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	// パイプランステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
	if (result != S_OK)
	{
		return false;
	}

	return true;
}

ID3D12PipelineState* KPipelineState::GetPipelineState()
{
	return pipelineState.Get();
}
