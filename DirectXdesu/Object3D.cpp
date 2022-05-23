#include "Object3D.h"

//Object3D::Object3D(HRESULT result, Object3d* object, ID3D12Device* dev) {
//	for (int i = 0; i < _countof(object); i++) {
//		Initialize(result, dev);
//
//		if (i > 0) {
//			object.parent = &object[i - 1];
//
//			object.scale = { 0.9f,0.9f,0.9f };
//			object.rot = { 0.0f,0.0f,XMConvertToRadians(30.0f) };
//			object.pos = { 0.0f,0.0f,-8.0f };
//		}
//	}
//}
//void Object3D::Initialize(HRESULT result, ID3D12Device* dev) {
//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
//	cbResourceDesc.Height = 1;
//	cbResourceDesc.DepthOrArraySize = 1;
//	cbResourceDesc.MipLevels = 1;
//	cbResourceDesc.SampleDesc.Count = 1;
//	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//	 定数バッファの生成
//	result = dev->CreateCommittedResource(
//		&cbHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&cbResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&object3d->constBuffTransform)
//	);
//	assert(SUCCEEDED(result));
//	 定数バッファのマッピング
//	result = object3d->constBuffTransform->Map(
//		0,
//		nullptr,
//		(void**)&object3d->constMapTransform
//	);
//	assert(SUCCEEDED(result));
//}
//
//void Object3D::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
//	XMMATRIX matScale, matRot, matTrans;
//
//	matScale = XMMatrixScaling(object3d->scale.x, object3d->scale.y, object3d->scale.z);
//	matRot = XMMatrixIdentity();
//	matRot *= XMMatrixRotationZ(object3d->rot.z);
//	matRot *= XMMatrixRotationX(object3d->rot.x);
//	matRot *= XMMatrixRotationY(object3d->rot.y);
//	matTrans = XMMatrixTranslation(
//		object3d->pos.x, object3d->pos.y, object3d->pos.z
//	);
//
//	object3d->matWorld = XMMatrixIdentity();
//	object3d->matWorld *= matScale;
//	object3d->matWorld *= matRot;
//	object3d->matWorld *= matTrans;
//
//	if (object3d->parent != nullptr) {
//		object3d->matWorld *= object3d->parent->matWorld;
//	}
//
//	object3d->constMapTransform->mat = object3d->matWorld * matView * matProjection;
//}
//
//void Object3D::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbview,
//	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices) {
//	cmdList->IASetVertexBuffers(0, 1, &vbview);
//	cmdList->IASetIndexBuffer(&ibView);
//	cmdList->SetGraphicsRootConstantBufferView(
//		2,
//		object3d->constBuffTransform->GetGPUVirtualAddress()
//	);
//	cmdList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
//}

