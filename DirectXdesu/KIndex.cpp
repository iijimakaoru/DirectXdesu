#include "KIndex.h"

KIndex::KIndex(KDirectInit dx, KVertex vertex) {
	IndexInit(dx, vertex);
}

void KIndex::IndexInit(KDirectInit dx, KVertex vertex) {
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	vertex.resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertex.resDesc.Width = sizeIB;
	vertex.resDesc.Height = 1;
	vertex.resDesc.DepthOrArraySize = 1;
	vertex.resDesc.MipLevels = 1;
	vertex.resDesc.SampleDesc.Count = 1;
	vertex.resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	dx.result = dx.dev->CreateCommittedResource(
		&vertex.heapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertex.resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	dx.result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// 全インデックスに対して
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	// マッピング解除
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}