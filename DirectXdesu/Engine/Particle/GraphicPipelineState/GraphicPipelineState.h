#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <cstdint>
#include <array>

#include "KShader.h"

struct RenderTargetFormat
{
	//描画対象数
	uint32_t NumRenderTargets;
	//描画対象のフォーマット
	std::array<DXGI_FORMAT, 8> RTVFormats = { DXGI_FORMAT_UNKNOWN };

	RenderTargetFormat();
};

class GraphicPipelineState
{
private:
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ID3D12RootSignature* rootSignature = nullptr;
	//頂点シェーダーオブジェクト
	KShader vsShader;
	//ピクセルシェーダーオブジェクト
	KShader psShader;
	//ジオメトリシェーダーオブジェクト
	KShader gsShader;

	//インプットレイアウトデータ
	D3D12_INPUT_ELEMENT_DESC* inputLayoutData;
	//インプットレイアウトサイズ
	size_t inputLayoutlength_;
	//ブレンドデスク
	D3D12_BLEND_DESC blendDesc = {};

	//プリミティブ形状
	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//カリングのモード
	D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK;

	D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID;

	//レンダーターゲットのフォーマット
	RenderTargetFormat renderTargetFormat;

	D3D12_DEPTH_WRITE_MASK depthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;

	//深度フラグ
	bool depthFlag = true;
	int8_t PADING[3]{};

public:

	/// <summary>
	/// インプットレイアウトをセット
	/// </summary>
	/// <param name="desc">インプットレイアウトの先頭アドレス</param>
	/// <param name="length_">要素数</param>
	void SetInputLayout(D3D12_INPUT_ELEMENT_DESC* desc_, size_t length_);

	/// <summary>
	/// ルートシグネチャのアドレスをセット
	/// </summary>
	/// <param name="pRootSignature">ルートシグネチャのアドレス</param>
	void SetRootSignature(ID3D12RootSignature* pRootSignature_);

	/// <summary>
	/// ブレンドをセット
	/// </summary>
	/// <param name="desc">ブレンドデスク</param>
	void SetBlend(const D3D12_BLEND_DESC& desc_);

	/// <summary>
	/// 深度テストのフラグをセット
	/// </summary>
	/// <param name="flag">深度テストのフラグ</param>
	void SetDepthFlag(bool flag_);

	void CreateVertexShader(LPCWSTR fileName, LPCSTR entryPoint);
	void CreatePixelShader(LPCWSTR fileName, LPCSTR entryPoint);
	void CreateGeometryShader(LPCWSTR fileName, LPCSTR entryPoint);

	/// <summary>
	/// プリミティブ形状をセット
	/// </summary>
	/// <param name="type">プリミティブ形状</param>
	void SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type_);

	/// <summary>
	/// カリングモードをセット
	/// </summary>
	/// <param name="mode">モード</param>
	void SetCullMode(D3D12_CULL_MODE mode_);

	/// <summary>
	/// フィルモードをセット
	/// </summary>
	/// <param name="mode">モード</param>
	void SetFillMode(D3D12_FILL_MODE mode_);

	void SetRenderTargetFormat(const RenderTargetFormat& format_);

	void SetDepthWriteMask(D3D12_DEPTH_WRITE_MASK depthWriteMask_);

	/// <summary>
	/// パイプラインステートを生成
	/// </summary>
	/// <returns>生成出来たか</returns>
	bool Create(ID3D12Device* device_);

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	ID3D12PipelineState* GetPipelineState()const;
};

