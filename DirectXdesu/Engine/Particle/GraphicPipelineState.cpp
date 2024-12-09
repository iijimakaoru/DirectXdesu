#include "GraphicPipelineState.h"
#include <cassert>
#include <string>

RenderTargetFormat::RenderTargetFormat()
{
	//描画対象は1つ
	NumRenderTargets = 1;
	//0～255指定のRGBA
	RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
}

void GraphicPipelineState::SetInputLayout(D3D12_INPUT_ELEMENT_DESC* desc_, size_t length_)
{
	inputLayoutData = desc_;
	inputLayoutlength_ = length_;
}

void GraphicPipelineState::SetRootSignature(ID3D12RootSignature* pRootSignature_)
{
	rootSignature = pRootSignature_;
}

void GraphicPipelineState::SetBlend(const D3D12_BLEND_DESC& desc_)
{
	blendDesc = desc_;
}

void GraphicPipelineState::SetDepthFlag(bool flag_)
{
	depthFlag = flag_;
}

void GraphicPipelineState::CreateVertexShader(LPCWSTR fileName, LPCSTR entryPoint)
{
	std::wstring ShaderHeadFileName = L"Resources/Shader/";
	ShaderHeadFileName += fileName;
	vsShader.Create(ShaderHeadFileName.c_str(), entryPoint, "vs_5_0");
}

void GraphicPipelineState::CreatePixelShader(LPCWSTR fileName, LPCSTR entryPoint)
{
	std::wstring ShaderHeadFileName = L"Resources/Shader/";
	ShaderHeadFileName += fileName;
	psShader.Create(ShaderHeadFileName.c_str(), entryPoint, "ps_5_0");
}

void GraphicPipelineState::CreateGeometryShader(LPCWSTR fileName, LPCSTR entryPoint)
{
	std::wstring ShaderHeadFileName = L"Resources/Shader/";
	ShaderHeadFileName += fileName;
	gsShader.Create(ShaderHeadFileName.c_str(), entryPoint, "gs_5_0");
}

void GraphicPipelineState::SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type_)
{
	primitiveType = type_;
}

void GraphicPipelineState::SetCullMode(D3D12_CULL_MODE mode_)
{
	cullMode = mode_;
}

void GraphicPipelineState::SetFillMode(D3D12_FILL_MODE mode_)
{
	fillMode = mode_;
}

void GraphicPipelineState::SetRenderTargetFormat(const RenderTargetFormat& format_)
{
	renderTargetFormat = format_;
}

void GraphicPipelineState::SetDepthWriteMask(D3D12_DEPTH_WRITE_MASK depthWriteMask_)
{
	depthWriteMask = depthWriteMask_;
}

bool GraphicPipelineState::Create(ID3D12Device* device_)
{
	HRESULT result;

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// シェーダーの設定
	if (vsShader.GetIsCreate())
	{
		pipelineDesc.VS = vsShader.GetBytecode();
	}
	if (psShader.GetIsCreate())
	{
		pipelineDesc.PS = psShader.GetBytecode();
	}
	if (gsShader.GetIsCreate())
	{
		pipelineDesc.GS = gsShader.GetBytecode();
	}

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = cullMode; // カリングしない
	pipelineDesc.RasterizerState.FillMode = fillMode; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	//ブレンドデスク
	pipelineDesc.BlendState = blendDesc;

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputLayoutlength_);
	pipelineDesc.InputLayout.pInputElementDescs = inputLayoutData;

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = primitiveType;
	//デプスステンシルステートの設定
	//深度テストを行うか
	pipelineDesc.DepthStencilState.DepthEnable = depthFlag;
	pipelineDesc.DepthStencilState.DepthWriteMask = depthWriteMask;//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度フォーマット

	// その他の設定

	//描画対象数
	pipelineDesc.NumRenderTargets = renderTargetFormat.NumRenderTargets;

	//描画対象のフォーマット
	std::memcpy(pipelineDesc.RTVFormats, renderTargetFormat.RTVFormats.data(), sizeof(DXGI_FORMAT) * 8);

	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	// パイプランステートの生成
	result = device_->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	if (result != S_OK)
	{
		return false;
	}

	return true;
}

ID3D12PipelineState* GraphicPipelineState::GetPipelineState() const
{
	return pipelineState.Get();
}
