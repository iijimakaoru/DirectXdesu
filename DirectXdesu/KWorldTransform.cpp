#include "KWorldTransform.h"

KWorldTransform::KWorldTransform(){}

KWorldTransform::KWorldTransform(ID3D12Device* dev) {
	Initialize(dev);
}

void KWorldTransform::Initialize(ID3D12Device* dev) {

	for (int i = 0; i < _countof(object3d); i++) {
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
		result = dev->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&object3d[i].constBuffTransform)
		);
		assert(SUCCEEDED(result));
		// 定数バッファのマッピング
		result = object3d[i].constBuffTransform->Map(
			0,
			nullptr,
			(void**)&object3d[i].constMapTransform
		);
		assert(SUCCEEDED(result));

		// 親オブジェクト以外の初期化
		if (i > 0) {
			object3d[i].parent = &object3d[i - 1];

			rotResult[i] = { 0,0,1 };
			object3d[i].scale = { 0.9f,0.9f,0.9f };
			object3d[i].pos = { 0.0f,0.0f,-10.0f };
			object3d[i].rot = { XMConvertToRadians(30.0f),0.0f,0.0f };
		}
	}
}

void KWorldTransform::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
	for (int i = 0; i < _countof(object3d); i++) {
		// マトリックス
		XMMATRIX matScale, matRot, matTrans;

		// 親オブジェクト要素
		matScale = XMMatrixScaling(object3d[i].scale.x, object3d[i].scale.y, object3d[i].scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(object3d[i].rot.z);
		matRot *= XMMatrixRotationX(object3d[i].rot.x);
		matRot *= XMMatrixRotationY(object3d[i].rot.y);
		matTrans = XMMatrixTranslation(
			object3d[i].pos.x, object3d[i].pos.y, object3d[i].pos.z
		);

		object3d[i].matWorld = XMMatrixIdentity();
		object3d[i].matWorld *= matScale;
		object3d[i].matWorld *= matRot;
		object3d[i].matWorld *= matTrans;

		if (object3d[i].parent != nullptr) {
			object3d[i].matWorld *= object3d[i].parent->matWorld;
		}

		object3d[i].constMapTransform->mat = object3d[i].matWorld * matView * matProjection;
	}
}

void KWorldTransform::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbview,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices) {
	for (int i = 0; i < _countof(object3d); i++) {
		cmdList->IASetVertexBuffers(0, 1, &vbview);
		cmdList->IASetIndexBuffer(&ibView);
		cmdList->SetGraphicsRootConstantBufferView(
			2,
			object3d[i].constBuffTransform->GetGPUVirtualAddress());
		cmdList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
	}
}
