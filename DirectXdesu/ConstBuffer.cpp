#include "ConstBuffer.h"
#include "KDirectXCommon.h"

ConstBuff::ConstBuff()
{
	// ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// リソース設定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;

	// 生成
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp, // 
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc, // 
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material)
	);
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = material->Map(0, nullptr, (void**)&mapMaterial); // マッピング
	assert(SUCCEEDED(result));

	ChangeColor(Vector4(1, 1, 1, 1));

	resourceDesc.Width = (sizeof(ConstBufferTimeMaterial) + 0xff) & ~0xff;

	// 生成
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp, // 
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc, // 
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&timeMaterial)
	);
	assert(SUCCEEDED(result));

	result = timeMaterial->Map(0, nullptr, (void**)&timeMapMaterial);
	assert(SUCCEEDED(result));

	// GPUのメモリにデータ転送
	timeMapMaterial->time = 0;
}

ConstBuff::~ConstBuff()
{
	material->Unmap(0, nullptr);
	timeMaterial->Unmap(0, nullptr);
}

void ConstBuff::ChangeColor(const Vector4& color)
{
	// GPUのメモリへデータ転送
	mapMaterial->color = color;
}

void ConstBuff::TimeUpdate()
{
	timeMapMaterial->time += 0.1f;
}

void ConstBuff::Update()
{
	// 定数バッファビューの設定コマンド
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(1, timeMaterial->GetGPUVirtualAddress());
}

void ConstBuff::SetBufferView()
{
	// 定数バッファビューの設定コマンド
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
}
