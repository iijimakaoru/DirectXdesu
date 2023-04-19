#include "KMaterial.h"
#include "KDirectXCommon.h"

KMaterial::~KMaterial()
{
}

void KMaterial::Initialize() {

	pipelineState = std::make_unique<KPipelineState>();

	// 頂点シェーダーを設定
	if (shader->GetVSBlob() != nullptr)
	{
		pipelineState->SetVertexShader(shader->GetVSBytecode());
	}

	// ハルシェーダーを設定
	if (shader->GetHSBlob() != nullptr)
	{
		pipelineState->SetHullShader(shader->GetHSBytecode());
	}

	// ドメインシェーダーを設定
	if (shader->GetDSBlob() != nullptr)
	{
		pipelineState->SetDomainShader(shader->GetDSBytecode());
	}

	// ジオメトリシェーダーを設定
	if (shader->GetGSBlob() != nullptr)
	{
		pipelineState->SetGeometryShader(shader->GetGSBytecode());
	}

	// ピクセルシェーダーを設定
	if (shader->GetPSBlob() != nullptr)
	{
		pipelineState->SetPixelShader(shader->GetPSBytecode());
	}

	//頂点レイアウトを設定
	pipelineState->SetInputLayout(inputLayouts.data(), inputLayouts.size());

	//ブレンドを設定
	pipelineState->SetBlend(blenddesc);

	//深度フラグを設定
	pipelineState->SetDepthFlag(depthFlag);

	//ルートシグネチャを設定
	pipelineState->SetRootSignature(rootSignature->GetRootSignature());

	//プリミティブ形状を設定
	pipelineState->SetPrimitiveType(primitiveType);

	pipelineState->SetCullMode(cullMode);

	//生成
	if (!pipelineState->Create())
	{
		printf("パイプラインステートが生成できませんでした");
		assert(0);
	}
}