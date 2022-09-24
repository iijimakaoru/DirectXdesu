#pragma once
#include "KVertex.h"
#include <string>

class KTexture
{
public:
	KTexture(ID3D12Device* dev, const wchar_t* msg,const wchar_t* msg2);
	void LoadTexture(const wchar_t* msg, const wchar_t* msg2);
	void GeneMipMap();
	void SetTextureBuff();
	void GeneTextureBuff(ID3D12Device* dev);
	void SendData();
	void SetDescHeap();
	void GeneDescHeap(ID3D12Device* dev);
	void GetSrvHandle(ID3D12Device* dev);
	void SetSRV();
	void CreateSRV(ID3D12Device* dev);

	TexMetadata metadata{};
	ScratchImage scraychImg{};
	TexMetadata metadata2{};
	ScratchImage scraychImg2{};

	ScratchImage mipChain{};
	ScratchImage mipChain2{};

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	D3D12_RESOURCE_DESC textureResourceDesc2{};
	// �e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* texBuff = nullptr;
	ID3D12Resource* texBuff2 = nullptr;
	// SRV�̍ő��
	const size_t kMaxSRVCount = 2056;
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	// �ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ID3D12DescriptorHeap* srvHeap = nullptr;
	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle2;
	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};

	UINT incrementSize;

	HRESULT result;
};

