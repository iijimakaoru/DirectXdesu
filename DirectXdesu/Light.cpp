#include "Light.h"

ID3D12Device* Light::device = nullptr;

void Light::Staticlnitialize(ID3D12Device* device)
{
	// 再初期化
	assert(!Light::device);
	// nullptrチェック
	assert(device);
	// 静的メンバ変数のセット
	Light::device = device;
}

void Light::Initialize()
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};

	// 定数バッファの生成
}
