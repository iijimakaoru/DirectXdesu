#include "RootSignature.h"

#include <d3dx12.h>
#include <assert.h>

void RootSignature::Add(RootType type_, uint32_t shaderRegister_, uint32_t registerSpace_)
{
	// パラメーター
	D3D12_ROOT_PARAMETER rootParam{};

	// すべてのシェーダーから見えるようにする
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// パラメーターのタイプ
	rootParam.ParameterType = static_cast<D3D12_ROOT_PARAMETER_TYPE>(type_);

	// バッファ番号
	rootParam.Descriptor.ShaderRegister = shaderRegister_;

	rootParam.Descriptor.RegisterSpace = registerSpace_;

	// 最後尾にプッシュ
	params_.push_back(std::move(rootParam));
}

void RootSignature::Add(RangeType type_, uint32_t shaderRegister_, uint32_t registerSpace_, uint32_t descriptorCount_)
{
	// レンジ
	D3D12_DESCRIPTOR_RANGE descRange{};
	
	// レンジタイプ
	descRange.RangeType = static_cast<D3D12_DESCRIPTOR_RANGE_TYPE>(type_);
	descRange.NumDescriptors = descriptorCount_;
	descRange.BaseShaderRegister = shaderRegister_;
	descRange.RegisterSpace = registerSpace_;
	descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	ranges_.push_back(std::move(descRange));

	uint32_t rangeIndex = uint32_t(params_.size());
	rangeLocation_.push_back(std::move(rangeIndex));

	D3D12_ROOT_PARAMETER rootParam{};

	// すべてのシェーダーから見えるようにする
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// バッファ番号
	rootParam.DescriptorTable.NumDescriptorRanges = 1;

	rootParam.DescriptorTable.pDescriptorRanges = nullptr;

	// 最後尾にプッシュ
	params_.push_back(std::move(rootParam));
}

void RootSignature::AddStaticSampler(uint32_t shaderRegister_, uint32_t registerSpace_, D3D12_FILTER filter_, AddressMode addressU_, AddressMode addressV_, AddressMode addressW_)
{
	// サンプラー
	CD3DX12_STATIC_SAMPLER_DESC sampler{};

	// 初期化
	sampler.Init(shaderRegister_,
		filter_,
		static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressU_),
		static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressV_),
		static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressW_));
	sampler.RegisterSpace = registerSpace_;

	// プッシュ
	samplers_.push_back(std::move(sampler));
}

void RootSignature::Clear()
{
	params_.clear();
	ranges_.clear();
	rangeLocation_.clear();
	samplers_.clear();
}

void RootSignature::Create(ID3D12Device* device_)
{
	// レンジのアドレス代入
	for (size_t i = 0; i < ranges_.size(); ++i)
	{
		uint32_t lIndex = rangeLocation_[i];

		params_[lIndex].DescriptorTable.pDescriptorRanges = &ranges_[i];
	}

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;

	// パラメーターが空じゃないとき
	if (!params_.empty())
	{
		rootSignatureDesc.pParameters = params_.data();
		rootSignatureDesc.NumParameters = static_cast<UINT>(params_.size());
	}

	// サンプラーが空じゃないとき
	if (!samplers_.empty())
	{
		rootSignatureDesc.pStaticSamplers = samplers_.data();
		rootSignatureDesc.NumStaticSamplers = static_cast<UINT>(samplers_.size());
	}

	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	Microsoft::WRL::ComPtr<ID3DBlob> blob, errorBlob;

	HRESULT result = 
		D3D12SerializeRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, &errorBlob);

	result = device_->CreateRootSignature(0,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		IID_PPV_ARGS(rootSignature_.GetAddressOf()));

	if (FAILED(result))
	{
		assert(0);
	}
}

ID3D12RootSignature* RootSignature::GetRootSignature()
{
	return rootSignature_.Get();
}
