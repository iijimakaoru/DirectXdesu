#include "KWorldTransform.h"

KWorldTransform::KWorldTransform() {}

KWorldTransform::KWorldTransform(ID3D12Device& dev) {
	Initialize(dev);
}

void KWorldTransform::SetTexture() {

}

void KWorldTransform::SetModel(KModel* model) {
	this->model = model;
}

void KWorldTransform::Initialize(ID3D12Device& dev) {
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
	result = dev.CreateCommittedResource(
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
}

void KWorldTransform::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
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
}

void KWorldTransform::Draw(ID3D12GraphicsCommandList* cmdList) {
	cmdList->IASetVertexBuffers(0, 1, &model->vertex->vbView);
	cmdList->IASetIndexBuffer(&model->vertex->ibView);
	cmdList->SetGraphicsRootConstantBufferView(2, transform.constBuffTransform->GetGPUVirtualAddress());
	cmdList->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}
