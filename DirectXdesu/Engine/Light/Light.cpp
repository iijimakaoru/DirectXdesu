#include "Light.h"
#include "KDirectXCommon.h"

Light* Light::Create() {
	// 生成
	Light* light = new Light();
	if (light == nullptr) {
		return nullptr;
	}

	// 初期化
	light->Init();

	return light;
}

void Light::Init() {
	// 定数バッファ生成
	CreateConstBuffer();

	// 定数バッファへ転送
	TransferConstBuffer();
}

void Light::Update() {
	if (isDirty) {
		TransferConstBuffer();
		isDirty = false;
	}
}

void Light::Draw(UINT rootParamIndex) {
	// 定数バッファビューをセット
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(
	    rootParamIndex, constBuff_->GetGPUVirtualAddress());
}

void Light::SetLightDir(const DirectX::XMVECTOR& lightDir) {
	lightDir_ = DirectX::XMVector3Normalize(lightDir);
	isDirty = true;
}

void Light::SetLightRGB(const DirectX::XMFLOAT3& lightRGB) {
	lightRGB_ = lightRGB;
	isDirty = true;
}

void Light::CreateConstBuffer() {
	HRESULT result;

	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 定数バッファ生成
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
	    &cbHeapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE, 
		&cbResourceDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
	    nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void Light::TransferConstBuffer() {
	HRESULT result;
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightv = DirectX::XMVectorNegate(lightDir_);
		constMap->lightColor = lightRGB_;
		constBuff_->Unmap(0, nullptr);
	}
}
