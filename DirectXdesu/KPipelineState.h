#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

class KPipelineState
{
private:
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ID3D12RootSignature* rootSignature = nullptr;
	//頂点シェーダーオブジェクト
	D3D12_SHADER_BYTECODE* vsByte = nullptr;
	//ピクセルシェーダーオブジェクト
	D3D12_SHADER_BYTECODE* psByte = nullptr;
	//ジオメトリシェーダーオブジェクト
	D3D12_SHADER_BYTECODE* gsByte = nullptr;
	//ハルシェーダーオブジェクト
	D3D12_SHADER_BYTECODE* hsByte = nullptr;
	//ドメインシェーダオブジェクト
	D3D12_SHADER_BYTECODE* dsByte = nullptr;
	//深度フラグ
	bool depthFlag = true;
	char PADING[7]{};

	//インプットレイアウトデータ
	D3D12_INPUT_ELEMENT_DESC* inputLayoutData;
	//インプットレイアウトサイズ
	size_t inputLayoutLength;
	//ブレンドデスク
	D3D12_BLEND_DESC blendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	//プリミティブ形状
	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


	D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK;
public:

	/// <summary>
	/// インプットレイアウトをセット
	/// </summary>
	/// <param name="desc">インプットレイアウトの先頭アドレス</param>
	/// <param name="length">要素数</param>
	void SetInputLayout(D3D12_INPUT_ELEMENT_DESC* desc, size_t length);

	/// <summary>
	/// ルートシグネチャのアドレスをセット
	/// </summary>
	/// <param name="pRootSignature">ルートシグネチャのアドレス</param>
	void SetRootSignature(ID3D12RootSignature* pRootSignature);

	/// <summary>
	/// ブレンドをセット
	/// </summary>
	/// <param name="desc">ブレンドデスク</param>
	void SetBlend(D3D12_BLEND_DESC& desc);

	/// <summary>
	/// 深度テストのフラグをセット
	/// </summary>
	/// <param name="flag">深度テストのフラグ</param>
	void SetDepthFlag(bool flag);

	/// <summary>
	/// 頂点シェーダをセット
	/// </summary>
	/// <param name="vsByte">頂点シェーダオブジェクト</param>
	void SetVertexShader(D3D12_SHADER_BYTECODE* pVsByte);

	/// <summary>
	/// ピクセルシェーダをセット
	/// </summary>
	/// <param name="psByte">ピクセルシェーダオブジェクト</param>
	void SetPixelShader(D3D12_SHADER_BYTECODE* pPsByte);

	/// <summary>
	/// ジオメトリシェーダをセット
	/// </summary>
	/// <param name="gsByte">ジオメトリシェーダオブジェクト</param>
	void SetGeometryShader(D3D12_SHADER_BYTECODE* pGsByte);

	/// <summary>
	/// ハルシェーダをセット
	/// </summary>
	/// <param name="hsByte">ハルシェーダオブジェクト</param>
	void SetHullShader(D3D12_SHADER_BYTECODE* pHsByte);

	/// <summary>
	/// ドメインシェーダをセット
	/// </summary>
	/// <param name="pDsByte">ドメインシェーダオブジェクト</param>
	void SetDomainShader(D3D12_SHADER_BYTECODE* pDsByte);

	/// <summary>
	/// プリミティブ形状をセット
	/// </summary>
	/// <param name="type">プリミティブ形状</param>
	void SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type);

	/// <summary>
	/// カリングモードをセット
	/// </summary>
	/// <param name="model">モード</param>
	void SetCullMode(D3D12_CULL_MODE model);

	/// <summary>
	/// パイプラインステートを生成
	/// </summary>
	/// <returns>生成出来たか</returns>
	bool Create();

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	ID3D12PipelineState* GetPipelineState();
};

