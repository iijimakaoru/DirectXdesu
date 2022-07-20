#include "KWorldTransform.h"

KWorldTransform::KWorldTransform(){}

KWorldTransform::KWorldTransform(ID3D12Device* dev) {
	Initialize(dev);
}

void KWorldTransform::SetModel(KModel* model) {
	this->model = model;
}

void KWorldTransform::Initialize(ID3D12Device* dev) {
		// �q�[�v�ݒ�
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		// ���\�[�X�ݒ�
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		// �萔�o�b�t�@�̐���
		result = dev->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&object3d.constBuffTransform)
		);
		assert(SUCCEEDED(result));
		// �萔�o�b�t�@�̃}�b�s���O
		result = object3d.constBuffTransform->Map(
			0,
			nullptr,
			(void**)&object3d.constMapTransform
		);
		assert(SUCCEEDED(result));
}

void KWorldTransform::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
		// �}�g���b�N�X
		XMMATRIX matScale, matRot, matTrans;

		// �e�I�u�W�F�N�g�v�f
		matScale = XMMatrixScaling(object3d.scale.x, object3d.scale.y, object3d.scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(object3d.rot.z);
		matRot *= XMMatrixRotationX(object3d.rot.x);
		matRot *= XMMatrixRotationY(object3d.rot.y);
		matTrans = XMMatrixTranslation(object3d.pos.x, object3d.pos.y, object3d.pos.z);

		object3d.matWorld = XMMatrixIdentity();
		object3d.matWorld *= matScale;
		object3d.matWorld *= matRot;
		object3d.matWorld *= matTrans;

		object3d.constMapTransform->mat = object3d.matWorld * matView * matProjection;
}

void KWorldTransform::Draw(ID3D12GraphicsCommandList* cmdList) {
		cmdList->IASetVertexBuffers(0, 1, &model->vertex->vbView);
		cmdList->IASetIndexBuffer(&model->vertex->ibView);
		cmdList->SetGraphicsRootConstantBufferView(2, object3d.constBuffTransform->GetGPUVirtualAddress());
		cmdList->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}
