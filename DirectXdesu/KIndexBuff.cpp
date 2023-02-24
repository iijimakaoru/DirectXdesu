#include "KIndexBuff.h"
#include "KDirectXCommon.h"

void KIndexBuff::Create(size_t length, const uint32_t* data)
{
	bufferLength = length;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// リソースの設定
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(length * sizeof(uint32_t));	// リソースの設定

	// リソースを生成
	HRESULT result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		printf("インデックスバッファリソースの生成に失敗");
		return;
	}

	// インデックスバッファビューの設定
	bufferView = {};
	bufferView.BufferLocation = buffer->GetGPUVirtualAddress();
	bufferView.Format = DXGI_FORMAT_R32_UINT;
	bufferView.SizeInBytes = static_cast<UINT>(length * sizeof(uint32_t));

	result = buffer->Map(0, nullptr, &bufferMappedPtr);
	if (FAILED(result))
	{
		printf("インデックスバッファマッピングに失敗");
		return;
	}

	// マッピングする
	if (data != nullptr)
	{
		// インデックスデータをマッピング先に設定
		memcpy(bufferMappedPtr, data, length * sizeof(uint32_t));

		// マッピング解除
		buffer->Unmap(0, nullptr);
	}

	isValid = true;
}

bool KIndexBuff::IsValid()
{
	return isValid;
}

D3D12_INDEX_BUFFER_VIEW KIndexBuff::GetView() const
{
	return bufferView;
}

void KIndexBuff::Update(void* data)
{
	memcpy(bufferMappedPtr, data, bufferLength * sizeof(uint32_t));
}
