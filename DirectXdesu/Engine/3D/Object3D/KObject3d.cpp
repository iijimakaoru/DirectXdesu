#include "KObject3d.h"
#include "KDirectXCommon.h"

KGPlin* KObject3d::pipeline = nullptr;

KObject3d::KObject3d() {
	Initialize();
}

void KObject3d::Initialize() {
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 定数バッファB1
	// リソース設定
	D3D12_RESOURCE_DESC b1ResourceDesc{};
	b1ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	b1ResourceDesc.Width = (sizeof(ConstBufferDataB1) + 0xff) & ~0xff;
	b1ResourceDesc.Height = 1;
	b1ResourceDesc.DepthOrArraySize = 1;
	b1ResourceDesc.MipLevels = 1;
	b1ResourceDesc.SampleDesc.Count = 1;
	b1ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&b1ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1));
	assert(SUCCEEDED(result));

	// 定数バッファB0
	// リソース設定
	D3D12_RESOURCE_DESC b0ResourceDesc{};
	b0ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	b0ResourceDesc.Width = (sizeof(ConstBufferDataB0) + 0xff) & ~0xff;
	b0ResourceDesc.Height = 1;
	b0ResourceDesc.DepthOrArraySize = 1;
	b0ResourceDesc.MipLevels = 1;
	b0ResourceDesc.SampleDesc.Count = 1;
	b0ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&b0ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));
	assert(SUCCEEDED(result));
}

void KObject3d::LoadModel(KModel* model)
{
	model_ = model;
}

void KObject3d::SetPipeline(KGPlin* pipeline_)
{
	KObject3d::pipeline = pipeline_;
}

void KObject3d::Update(ViewProjection* viewProjection) {
	// マトリックス
	XMMATRIX matScale, matRot, matTrans;

	// 親オブジェクト要素
	matScale = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(transform.rot.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(transform.rot.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(transform.rot.y));
	matTrans = XMMatrixTranslation(transform.pos.x, transform.pos.y, transform.pos.z);

	transform.matWorld = XMMatrixIdentity();
	transform.matWorld *= matScale;
	transform.matWorld *= matRot;
	transform.matWorld *= matTrans;

	// 定数バッファのマッピング
	// B1
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = model_->objMtl.ambient;
	constMap1->diffuse = model_->objMtl.diffuse;
	constMap1->specular = model_->objMtl.specular;
	constMap1->alpha = model_->objMtl.alpha;
	constBuffB1->Unmap(0, nullptr);
	assert(SUCCEEDED(result));
	// B0
	ConstBufferDataB0* constMap0 = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap0);
	constMap0->mat = transform.matWorld * viewProjection->matView * viewProjection->matProjection;
	constBuffB0->Unmap(0, nullptr);
	assert(SUCCEEDED(result));
}

void KObject3d::Draw() 
{
	pipeline->Setting();
	pipeline->Update(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 定数バッファビューをセット
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(1, constBuffB0->GetGPUVirtualAddress());
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(2, constBuffB1->GetGPUVirtualAddress());

	model_->Draw();
}

void KObject3d::Draw(KTexture* texture)
{
	assert(texture);

	pipeline->Setting();
	pipeline->Update(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 定数バッファビューをセット
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(1, constBuffB0->GetGPUVirtualAddress());
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(2, constBuffB1->GetGPUVirtualAddress());

	model_->Draw(texture);
}

void KObject3d::Finalize()
{
	constBuffB0->Release();
	constBuffB1->Release();
}

void KObject3d::StaticInitialize()
{
	
}

KObject3d KObject3d::Create()
{
	return KObject3d();
}
