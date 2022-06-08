#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Vector3.h"

using namespace DirectX;

// �萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	XMMATRIX mat; // 3D�ϊ��s��
};

// 3D�I�u�W�F�N�g�̐�
const size_t kObjectCount = 1;

struct Object3d {
	// �萔�o�b�t�@(�s��)
	ID3D12Resource* constBuffTransform = {};
	// �萔�o�b�t�@�}�b�v(�s��)
	ConstBufferDataTransform* constMapTransform = {};
	// �A�t�B���ϊ�
	Vector3 scale = { 1,1,1 };
	Vector3 rot = { 0,0,0 };
	Vector3 pos = { 0,0,0 };
	// ���[���h�ϊ��s��
	XMMATRIX matWorld = {};
	// �e�I�u�W�F�N�g�ւ̃|�C���^
	Object3d* parent = nullptr;
};

class KWorldTransform
{
public:
	KWorldTransform();
	KWorldTransform(HRESULT result, ID3D12Device* dev);
	void Initialize(HRESULT result, ID3D12Device* dev);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbview,
		D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);

	Vector3 rotResult[kObjectCount];
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	// 3D�I�u�W�F�N�g�̔z��
	Object3d object3d[kObjectCount];
};

