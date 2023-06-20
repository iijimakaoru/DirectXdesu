#pragma once
#include "KVertex.h"
#include <DirectXTex.h>
#include <string>
#include <wrl.h>
#include <map>
#include <mutex>

#include "Vector4.h"

using namespace Microsoft::WRL;

using namespace DirectX;

using namespace std;

class KTexture
{
public:
	KTexture() {};
	void CreateTexture(const std::string& directoryPath, const std::string& filename);

	TexMetadata metadata{};

	ScratchImage scraychImg{};

	ScratchImage mipChain{};

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};

	// �e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff;

	// SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	// �ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	UINT incrementSize;

	HRESULT result;
};

class KTextureManager
{
public:
	//TextureManager���擾����
	static KTextureManager* GetInstance() 
	{
		if (textureManager == nullptr)
		{
			textureManager = new KTextureManager();
		}

		return textureManager;
	}

	// ���
	void Destory()
	{
		delete textureManager;
	}

private:
	KTextureManager() {
		Init();
	};
	~KTextureManager() = default;
	KTextureManager(const KTextureManager & a) {};
	KTextureManager& operator=(const KTextureManager&) { return *this; }

	static KTextureManager* textureManager;

	void Init();

public:
	int textureNum;
};