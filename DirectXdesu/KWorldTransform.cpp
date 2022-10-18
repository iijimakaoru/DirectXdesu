#include "KWorldTransform.h"

KWorldTransform::KWorldTransform() {}

KWorldTransform::KWorldTransform(ID3D12Device* dev) {
	Initialize(dev);
}

void KWorldTransform::SetTexture(KTexture* texture) {
	this->texture = texture;
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
		IID_PPV_ARGS(&transform.constBuffTransform));
	assert(SUCCEEDED(result));
	// �萔�o�b�t�@�̃}�b�s���O
	result = transform.constBuffTransform->Map(0, nullptr, (void**)&transform.constMapTransform);
	assert(SUCCEEDED(result));

	material = new KMaterial(dev);
}

void KWorldTransform::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
	// �}�g���b�N�X
	XMMATRIX matScale, matRot, matTrans;

	// �e�I�u�W�F�N�g�v�f
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

void KWorldTransform::Draw(ID3D12GraphicsCommandList* cmdList) {
	// CBV
	cmdList->SetGraphicsRootConstantBufferView(0, material->constBufferMaterial->GetGPUVirtualAddress());
	// SRV
	cmdList->SetDescriptorHeaps(1, &texture->srvHeap);
	// �擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texture->srvHeap->GetGPUDescriptorHandleForHeapStart();
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃ̐ݒ�
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	cmdList->IASetVertexBuffers(0, 1, &model->vertex->vbView);
	cmdList->IASetIndexBuffer(&model->vertex->ibView);
	cmdList->SetGraphicsRootConstantBufferView(2, transform.constBuffTransform->GetGPUVirtualAddress());
	cmdList->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}

void KWorldTransform::SecoundDraw(ID3D12GraphicsCommandList* cmdList)
{
	// CBV
	cmdList->SetGraphicsRootConstantBufferView(0, material->constBufferMaterial->GetGPUVirtualAddress());
	// SRV
	cmdList->SetDescriptorHeaps(1, &texture->srvHeap);
	// �擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texture->srvHeap->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle.ptr += texture->incrementSize * 1;
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃ̐ݒ�
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	cmdList->IASetVertexBuffers(0, 1, &model->vertex->vbView);
	cmdList->IASetIndexBuffer(&model->vertex->ibView);
	cmdList->SetGraphicsRootConstantBufferView(2, transform.constBuffTransform->GetGPUVirtualAddress());
	cmdList->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}
