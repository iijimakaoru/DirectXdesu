#pragma once
#include "KDirectInit.h"
#include "KVertex.h"

class KTexture
{
public:
	KTexture(KDirectInit dx, KVertex vertex);
	void LoadTexture(KDirectInit dx,int i);
	void GeneMipMap(KDirectInit dx);
	void SetTextureBuff();
	void GeneTextureBuff(KDirectInit dx);
	void SendData(KDirectInit dx);
	void SetDescHeap();
	void GeneDescHeap(KDirectInit dx);
	void GetSrvHandle();
	void SetSRV(KDirectInit dx,KVertex vertex);
	void CreateSRV(KDirectInit dx);

	TexMetadata metadata{};
	ScratchImage scraychImg{};

	ScratchImage mipChain{};

	const wchar_t* msg[2] = { L"Resources/kitanai.jpg",L"Resources/haikei.jpg" };

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
};
