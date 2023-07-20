#pragma once
#include <string>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <vector>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include "KVertex.h"

#include <fbxsdk.h>

#include "TextureManager.h"

// �m�[�h
struct Node
{
	// ���O
	std::string name;
	// ���[�J���X�P�[��
	KMyMath::Vector4 scaling = { 1,1,1,0 };
	// ���[�J����]�p
	KMyMath::Vector4 rotation = { 0,0,0,0 };
	// ���[�J���ړ�
	KMyMath::Vector4 translation = { 0,0,0,1 };
	// ���[�J���ό`�s��
	KMyMath::Matrix4 transform;
	// �O���[�o���ό`�s��
	KMyMath::Matrix4 globalTransform;
	// �e�m�[�h
	Node* parent = nullptr;
};

class FbxModel
{
public:
	// �t�����h�N���X
	friend class FbxLoader;

	static const int MAX_BONE_INDICES = 4;

	struct Bone
	{
		std::string name;
		// �����p���̋t�s��
		KMyMath::Matrix4 invInitialPose;
		// �N���X�^�[
		FbxCluster* fbxCluster;
		// �R���g���N�^
		Bone(const std::string& name_)
		{
			name = name_;
		}
	};

	struct VertexPosNormalUVSkin
	{
		KMyMath::Vector3 pos;	 // xyz���W
		KMyMath::Vector3 normal; // �@���x�N�g��
		KMyMath::Vector2 uv;	 // uv���W
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::�ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	// std::�ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;

private:
	// ���f����
	std::string name;
	// �m�[�h�z��
	std::vector<Node> nodes;
	// ���b�V�������m�[�h
	Node* meshNode = nullptr;
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUVSkin> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	// �A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	// �f�B�t�[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };

	std::unique_ptr<KVertex> vertexs = std::make_unique<KVertex>();

	ScratchImage scratchImg;

	TexMetadata metadata;

	// 
	ComPtr<ID3D12Resource> vertBuff;

	ComPtr<ID3D12Resource> indexBuff;

	// �e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff;

	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// �ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	//KTexture texture;

	std::vector<Bone> bones;

	// FBX�V�[��
	FbxScene* fbxScene = nullptr;

public: // �֐�
	// �����o
	void CreateBuffer();
	void LoadTexture();
	void Draw();

	~FbxModel();

	FbxScene* GetFbxScene() { return fbxScene; }

	// �Q�b�^�[
	const KMyMath::Matrix4& GetModelTransform() { return meshNode->globalTransform; }
	std::vector<Bone>& GetBones() { return bones; }
};

