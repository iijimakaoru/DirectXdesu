#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>

using namespace Microsoft::WRL;

using namespace DirectX;

// �萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // �F
};

class KMaterial
{
public:
	KMaterial();
	KMaterial(ID3D12Device* dev);
	void Initialize(ID3D12Device* dev);
	void Update();

	// �摜�̐F
	float colorR = 1.0f;
	float colorG = 1.0f;
	float colorB = 1.0f;
	float colorA = 1.0f;
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	// �萔�o�b�t�@�̐���
	ComPtr<ID3D12Resource> constBufferMaterial;
	// �萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	HRESULT result;
};

