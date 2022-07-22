#include "KMaterial.h"

KMaterial::KMaterial(ID3D12Device* dev) {
	Initialize(dev);
}

KMaterial::KMaterial() {

}

void KMaterial::Initialize(ID3D12Device* dev) {
	// 画像の色
	colorR = 0.5f;
	colorG = 0.5f;
	colorB = 0.5f;
	colorA = 1.0f;
	// ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferMaterial));
	assert(SUCCEEDED(result));
	// 定数バッファのマッピング
	result = constBufferMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));
	// 値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(colorR, colorG, colorB, colorA);
}

void KMaterial::Update() {
	constMapMaterial->color = XMFLOAT4(colorR, colorG, colorB, colorA);
}