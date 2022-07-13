#pragma once
#include "KVertex.h"

class KTexture
{
public:
	KTexture(ID3D12Device* dev,KVertex vertex);
	void LoadTexture();
	void GeneMipMap();
	void SetTextureBuff();
	void GeneTextureBuff(ID3D12Device* dev);
	void SendData();
	void SetDescHeap();
	void GeneDescHeap(ID3D12Device* dev);
	void GetSrvHandle();
	void SetSRV(KVertex vertex);
	void CreateSRV(ID3D12Device* dev);

	TexMetadata metadata{};
	ScratchImage scraychImg{};

	ScratchImage mipChain{};

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	// �e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* texBuff = nullptr;
	// SRV�̍ő��
	const size_t kMaxSRVCount = 2056;
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	// �ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ID3D12DescriptorHeap* srvHeap = nullptr;
	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	HRESULT result;
};

