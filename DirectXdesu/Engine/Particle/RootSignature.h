#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <vector>

enum class RootType 
{
	CBV = D3D12_ROOT_PARAMETER_TYPE_CBV,
	SRV = D3D12_ROOT_PARAMETER_TYPE_SRV,
	UAV = D3D12_ROOT_PARAMETER_TYPE_UAV,
};

enum class RangeType 
{
	CBV = D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
	SRV = D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
	UAV = D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
	Sampler = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
};

enum class AddressMode
{
	Wrap = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
	Mirror = D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
	Clamp = D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
	Border = D3D12_TEXTURE_ADDRESS_MODE_BORDER,
	MirrorOnce = D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
};

class RootSignature
{
public:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	std::vector<D3D12_ROOT_PARAMETER> params_;
	std::vector<D3D12_DESCRIPTOR_RANGE> ranges_;
	std::vector<uint32_t>rangeLocation_;
	std::vector<D3D12_STATIC_SAMPLER_DESC>samplers_;

public:
	/// <summary>
	/// ルートパラメータを追加
	/// </summary>
	void Add(RootType type_, uint32_t shaderRegister_, uint32_t registerSpace_ = 0);

	/// <summary>
	/// レンジを追加
	/// </summary>
	void Add(RangeType type_, uint32_t shaderRegister_, uint32_t registerSpace_ = 0, uint32_t descriptorCount_ = 1);

	/// <summary>
	/// サンプラーを追加
	/// </summary>
	void AddStaticSampler(
		uint32_t shaderRegister_,
		uint32_t registerSpace_ = 0,
		D3D12_FILTER filter_ = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		AddressMode addressU_ = AddressMode::Wrap,
		AddressMode addressV_ = AddressMode::Wrap,
		AddressMode addressW_ = AddressMode::Wrap
	);

	/// <summary>
	/// クリア
	/// </summary>
	void Clear();

	/// <summary>
	/// 生成
	/// </summary>
	void Create(ID3D12Device* device_);

	/// <summary>
	/// ルートシグネチャ取得
	/// </summary>
	ID3D12RootSignature* GetRootSignature();

	RootSignature() = default;
	~RootSignature() = default;
};

