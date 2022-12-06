#include "KObject3d.h"
#include "KDirectXCommon.h"

KObject3d::KObject3d() {
	Initialize();
}

void KObject3d::Initialize() {
	// ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 定数バッファの生成
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform.constBuffTransform));
	assert(SUCCEEDED(result));
	// 定数バッファのマッピング
	result = transform.constBuffTransform->Map(0, nullptr, (void**)&transform.constMapTransform);
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

	transform.constMapTransform->mat = transform.matWorld * matView * matProjection;

	material->Update();
}

void KObject3d::Draw(KTexture* texture, KModel* model) {
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
	// SRVヒープの先頭にあるSRVをルートパラメータ1番の設定
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	KDirectXCommon::GetInstance()->GetCmdlist()->IASetVertexBuffers(0, 1, &model->vertexs->vbView);
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetIndexBuffer(&model->vertexs->ibView);
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(2, transform.constBuffTransform->GetGPUVirtualAddress());
	KDirectXCommon::GetInstance()->GetCmdlist()->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
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
