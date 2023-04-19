#include "KRootSignature.h"
#include "KDirectXCommon.h"

void KRootSignature::Add(RootType type, UINT shaderRegister, UINT registerSpace)
{
	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParam{};
	//全てのシェーダから見える
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//タイプ
	rootParam.ParameterType = static_cast<D3D12_ROOT_PARAMETER_TYPE>(type);
	//バッファ番号
	rootParam.Descriptor.ShaderRegister = shaderRegister;
	//よくわからん
	rootParam.Descriptor.RegisterSpace = registerSpace;
	params.push_back(rootParam);
}

void KRootSignature::Add(RangeType type, UINT shaderRegister, UINT registerSpace, UINT descriptorCount)
{
	D3D12_DESCRIPTOR_RANGE range{};
	//タイプ
	range.RangeType = static_cast<D3D12_DESCRIPTOR_RANGE_TYPE>(type);
	range.NumDescriptors = descriptorCount;
	range.BaseShaderRegister = shaderRegister;
	range.RegisterSpace = registerSpace;
	range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	ranges.push_back(range);

	// あとで Range 解決するのでインデックスを保存.
	UINT rangeIndex = UINT(params.size());
	rangeLocation.push_back(rangeIndex);

	D3D12_ROOT_PARAMETER rootParam{};
	//全てのシェーダから見える
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//タイプ
	rootParam.DescriptorTable.NumDescriptorRanges = 1;
	//レンジ
	rootParam.DescriptorTable.pDescriptorRanges = nullptr; // ここは生成時入れる.
	params.push_back(rootParam);
}

void KRootSignature::AddStaticSampler(UINT shaderRegister, UINT registerSpace, D3D12_FILTER filter, AddressMode addressU, AddressMode addressV, AddressMode addressW)
{
	CD3DX12_STATIC_SAMPLER_DESC desc{};
	desc.Init(shaderRegister,
		filter,
		static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressU),
		static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressV),
		static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressW)
	);
	desc.RegisterSpace = registerSpace;
	samplers.push_back(desc);
}

void KRootSignature::Clear()
{
	params.clear();
	ranges.clear();
	rangeLocation.clear();
	samplers.clear();
}

void KRootSignature::Create(ID3D12Device* device)
{
	for (UINT i = 0; i < UINT(ranges.size()); ++i)
	{
		auto index = rangeLocation[i];
		//レンジのアドレス代入
		params[index].DescriptorTable.pDescriptorRanges = &ranges[i];
	}
	D3D12_ROOT_SIGNATURE_DESC desc{};
	if (!params.empty())
	{
		desc.pParameters = params.data();
		desc.NumParameters = UINT(params.size());
	}
	if (!samplers.empty())
	{
		desc.pStaticSamplers = samplers.data();
		desc.NumStaticSamplers = UINT(samplers.size());
	}
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	Microsoft::WRL::ComPtr<ID3DBlob> blob, errBlob;
	HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, &errBlob);

	hr = device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(rootSignature.GetAddressOf()));
	if (FAILED(hr))
	{
		assert(0);
	}
}

ID3D12RootSignature* KRootSignature::GetRootSignature()
{
	return rootSignature.Get();
}
