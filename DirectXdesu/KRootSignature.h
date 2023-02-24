#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <vector>

class KRootSignature
{
public:
	/// <summary>
	/// ルートパラメータタイプ
	/// </summary>
	enum class RootType
	{
		CBV = D3D12_ROOT_PARAMETER_TYPE_CBV,
		SRV = D3D12_ROOT_PARAMETER_TYPE_SRV,
		UAV = D3D12_ROOT_PARAMETER_TYPE_UAV,
	};

	/// <summary>
	/// レンジタイプ
	/// </summary>
	enum class RangeType
	{
		SRV = D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		UAV = D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
		CBV = D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
		Sampler = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER
	};
	enum class AddressMode
	{
		Wrap = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		Mirror = D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
		Clamp = D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		Border = D3D12_TEXTURE_ADDRESS_MODE_BORDER,
		MirrorOnce = D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE
	};

	/// <summary>
	/// ルートパラメータを追加
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="shaderRegister">バッファ番号</param>
	/// <param name="registerSpace">レジスタースペース(基本的に0でいいような気がする)</param>
	void Add(RootType type, UINT shaderRegister, UINT registerSpace = 0);

	/// <summary>
	/// レンジを追加
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="shaderRegister">バッファ番号</param>
	/// <param name="registerSpace">レジスタースペース(基本的に0でいいような気がする)</param>
	/// <param name="descriptorCount">一度に使うテクスチャや定数バッファ</param>
	void Add(RangeType type, UINT shaderRegister, UINT registerSpace = 0, UINT descriptorCount = 1);

	/// <summary>
	/// サンプラーを追加
	/// </summary>
	/// <param name="shaderRegister">バッファ番号</param>
	/// <param name="registerSpace">レジスタースペース(基本的に0でいいような気がする)</param>
	/// <param name="filter">フィルター</param>
	/// <param name="addressU">Uのアドレスモード</param>
	/// <param name="addressV">Vのアドレスモード</param>
	/// <param name="addressW">Wのアドレスモード</param>
	void AddStaticSampler(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_FILTER filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		AddressMode addressU = AddressMode::Wrap,
		AddressMode addressV = AddressMode::Wrap,
		AddressMode addressW = AddressMode::Wrap
	);

	/// <summary>
	/// 生成
	/// </summary>
	void Clear();

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="device">デバイス</param>
	void Create(ID3D12Device* device);

	/// <summary>
	/// ルートシグネチャ取得
	/// </summary>
	ID3D12RootSignature* GetRootSignature();

	KRootSignature() = default;
	~KRootSignature() = default;

private:

	/// <summary>
	/// パラメーター
	/// </summary>
	std::vector<D3D12_ROOT_PARAMETER>params;

	/// <summary>
	/// レンジ
	/// </summary>
	std::vector<D3D12_DESCRIPTOR_RANGE>ranges;


	std::vector<UINT>rangeLocation;

	/// <summary>
	/// サンプラー
	/// </summary>
	std::vector<D3D12_STATIC_SAMPLER_DESC>samplers;

	/// <summary>
	/// ルートシグネチャ
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
};

