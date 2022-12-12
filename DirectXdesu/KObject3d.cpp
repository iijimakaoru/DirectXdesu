#include "KObject3d.h"
#include "KDirectXCommon.h"
#include "ConstBuffer.h"

KObject3d::KObject3d() {
	Initialize();
}

void KObject3d::Initialize() {
	//// ヒープ設定
	//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//// リソース設定
	//cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	//cbResourceDesc.Height = 1;
	//cbResourceDesc.DepthOrArraySize = 1;
	//cbResourceDesc.MipLevels = 1;
	//cbResourceDesc.SampleDesc.Count = 1;
	//cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	
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

	material = new KMaterial();
}

void KObject3d::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
	// マトリックス
	XMMATRIX matScale, matRot, matTrans;

	// 親オブジェクト要素
	matScale = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(transform.rot.z);
	matRot *= XMMatrixRotationX(transform.rot.x);
	matRot *= XMMatrixRotationY(transform.rot.y);
	matTrans = XMMatrixTranslation(transform.pos.x, transform.pos.y, transform.pos.z);

	transform.matWorld = XMMatrixIdentity();
	transform.matWorld *= matScale;
	transform.matWorld *= matRot;
	transform.matWorld *= matTrans;

	material->Update();

	// 定数バッファのマッピング
	// B1
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = model->objMtl.ambient;
	constMap1->diffuse = model->objMtl.diffuse;
	constMap1->specular = model->objMtl.specular;
	constMap1->alpha = model->objMtl.alpha;
	constBuffB1->Unmap(0, nullptr);
	assert(SUCCEEDED(result));
	// B0
	ConstBufferDataB0* constMap0 = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap0);
	constMap0->mat = transform.matWorld * matView * matProjection;
	constBuffB0->Unmap(0, nullptr);
	assert(SUCCEEDED(result));
}

void KObject3d::Draw(KTexture* texture, KModel* model) 
{
	// テクスチャの読み込み
	this->texture = texture;

	// モデルの読み込み
	this->model = model;

	// SRV
	KDirectXCommon::GetInstance()->GetCmdlist()->SetDescriptorHeaps(1, &texture->srvHeap);

	// 先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texture->srvHeap->GetGPUDescriptorHandleForHeapStart();

	// CBV
	//KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(0, material->constBufferMaterial->GetGPUVirtualAddress());
	
	// 頂点バッファビューの設定
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetVertexBuffers(0, 1, &this->model->vertexs->vbView);

	// インデックスバッファビューの設定
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetIndexBuffer(&this->model->vertexs->ibView);

	//KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(2, transform.constBuffTransform->GetGPUVirtualAddress());

	// 定数バッファビューをセット
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(2, constBuffB1->GetGPUVirtualAddress());

	// シェーダーリソースビューをセット
	//KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// 描画
	KDirectXCommon::GetInstance()->GetCmdlist()->DrawIndexedInstanced(this->model->indices.size(), 1, 0, 0, 0);
}

void KObject3d::SecoundDraw(KTexture* texture, KModel* model)
{
	// テクスチャの読み込み
	this->texture = texture;
	// モデルの読み込み
	this->model = model;
	// CBV
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(0, material->constBufferMaterial->GetGPUVirtualAddress());
	// SRV
	KDirectXCommon::GetInstance()->GetCmdlist()->SetDescriptorHeaps(1, &texture->srvHeap);
	// 先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texture->srvHeap->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle.ptr += texture->incrementSize * 1;
	// SRVヒープの先頭にあるSRVをルートパラメータ1番の設定
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetVertexBuffers(0, 1, &model->vertexs->vbView);
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetIndexBuffer(&model->vertexs->ibView);
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(2, transform.constBuffTransform->GetGPUVirtualAddress());
	KDirectXCommon::GetInstance()->GetCmdlist()->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}
