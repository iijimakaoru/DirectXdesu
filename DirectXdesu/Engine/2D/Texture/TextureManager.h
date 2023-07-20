#pragma once
#include <d3dx12.h>
#include <DirectXTex.h>
#include "MyMath.h"

struct TextureData
{
	// �e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

	// GPU�f�X�N���v�^�n���h��
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

	// �f�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	// ����
	size_t width = 0;

	// �c��
	size_t height = 0;
};

class TextureManager
{
public: // �萔
	static const size_t MaxSRVCount = 256;

public:
	// ������
	void Init();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	TextureData LoadTexture(const std::string& fileName);

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static TextureData Load(const std::string& fileName);

	// ���
	void Delete();

	// �C���X�^���X
	static TextureManager* GetInstance();

	// �Q�b�^�[
	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap.Get(); }

private:
	/// <summary>
	/// �e�N�X�`���o�b�t�@�̐���
	/// </summary>
	/// <param name="metadata"></param>
	/// <param name="scratchImg"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTexBuff(DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg);

	/// <summary>
	/// �V�F�[�_���\�[�X�r���[�̐���
	/// </summary>
	/// <param name="texBuff"></param>
	/// <param name="metadata"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE CreateSRV(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata);

private:
	ID3D12Device* device = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	D3D12_HEAP_PROPERTIES textureHeapProp{};

	// �e�N�X�`���o�b�t�@
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, MaxSRVCount> texBuff_;

	// �e�N�X�`����
	UINT texCount;

	// �f�t�H���g�e�N�X�`���i�[�f�B���N�g��
	static std::string DefaultTextureDirectoryPath;

	static TextureManager* textureManager;

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager(const TextureManager&) = delete;
};

