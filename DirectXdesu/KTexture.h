#pragma once
#include "KVertex.h"
#include <DirectXTex.h>
#include <string>
#include <wrl.h>

using namespace Microsoft::WRL;

using namespace DirectX;

using namespace std;

class KTexture
{
public:
	KTexture() {};
	void CreateTexture(const std::string& directoryPath, const std::string& filename);
	void LoadTexture(const std::string& directoryPath, const std::string& filename);
	void GeneMipMap();
	void SetTextureBuff();
	void GeneTextureBuff();
	void SendData();
	void SetDescHeap();
	void GeneDescHeap();
	void GetSrvHandle();
	void SetSRV();
	void CreateSRV();

	TexMetadata metadata{};
	ScratchImage scraychImg{};

	ScratchImage mipChain{};

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	// �e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff;
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

