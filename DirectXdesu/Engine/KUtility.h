#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "KVertexBuff.h"
#include "KIndexBuff.h"
#include "KComputePipelineState.h"
#include "KShader.h"

struct KTextureData
{
	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	//�f�X�N�v���^�q�[�v
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> srvHeap;

	//GPU�f�X�N�v���^�n���h��
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

	//����
	size_t width = 0;
	//�c��
	size_t height = 0;

	//�J���[
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	std::string path;

	uint32_t textureHandle;

	~KTextureData();

private:
	char PADING[4]{};
};

// ���W�ϊ��s��
struct KConstBufferDataTransform
{
	Matrix mat; // 3D�ϊ��s��
};

struct KPosColor
{
	Vector3 pos; // ���W
	Vector4 color; // �J���[
};

struct KPosNormalUV
{
	Vector3 pos; // ���W
	Vector3 normal; // �@���x�N�g��
	Vector2 uv; // uv���W
};

// ���W�EUV�E�J���[
struct KPosUvColor
{
	Vector3 pos; // ���W
	Vector2 uv; // uv���W
	Vector4 color; // �J���[
};

typedef struct
{
	// �p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	// �f�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	// �X�v���C�g��
	UINT nextIndex;
}KModelShareVaria;

struct KPipelineSet
{
	// �p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

struct KworldViewpojCamera
{
	//���[���h�s��
	Matrix world;
	//���[���h���W
	Matrix matWorld;
	//�J�������W(���[���h���W)
	Vector3 cameraPos;
};

struct worldViewpojCameraWVP
{
	//���[���h�s��
	Matrix world;
	//���[���h���W
	Matrix matWorld;
	//�J�������W(���[���h���W)
	Vector3 cameraPos;
	Matrix WVP;
};

//�}�e���A���悤�萔�o�b�t�@�\����
struct KConstBuffDataMaterial
{
	//�A���r�G���g�W��
	Vector3 ambient;
	//�p�f�B���O
	float pad1;
	//�f�B�t���[�Y�W��
	Vector3 diffuse;
	//�p�f�B���O
	float pad2;
	//�X�y�L�����[�W��
	Vector3 specular;
	//�A���t�@
	float alpha;
};

//�}�e���A��
struct KModelMaterial
{
	//�}�e���A����
	std::string name;
	//�A���r�G���g�e���x
	Vector3 ambient = { 0.06f, 0.06f, 0.06f };
	//�f�B�t���[�Y�e���x
	Vector3 diffuse = { 0.0f,0.0f,0.0f };
	//�X�y�L�����[�e���x
	Vector3 specular = { 0.0f,0.0f,0.0f };

	Vector3 emission = { 0.0f,0.0f,0.0f };

	float shininess = 1.0f;
	//�A���t�@
	float alpha = 1.0f;
	//�e�N�X�`���t�@�C����
	std::string textureFiename;

	KConstBuffDataMaterial GetConstBuffMaterial();
};

//�o�b�t�@�֘A
struct KBuff
{
	//���_�o�b�t�@
	std::unique_ptr<KVertexBuff> vertexBuffer;
	//���_�}�b�v
	KPosColor* vertMap;
	//�C���f�b�N�X�o�b�t�@
	std::unique_ptr<KIndexBuff> indexBuffer;
	//�C���f�b�N�X�}�b�v
	uint32_t* indexMap;

	KBuff() = default;
	~KBuff() = default;

private:
	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	KBuff& operator=(const KBuff&) = delete;
	KBuff(const KBuff&) = delete;

};

//�u�����h���[�h
enum class BlendMode
{
	AX_BLENDMODE_NOBLEND,//�m�[�u�����h�i�f�t�H���g�j
	AX_BLENDMODE_ALPHA,//���u�����h
	AX_BLENDMODE_ADD,//���Z�u�����h
	AX_BLENDMODE_SUB,//���Z�u�����h
	AX_BLENDMODE_MULA,//��Z�u�����h
	AX_BLENDMODE_INVSRC,//���]�u�����h

	AX_BLENDMODE_MAX,//�u�����h��ސ�

	AX_BLENDMODE_CUSTOM = -1,
};

//�R���s���[�g�V�F�[�_�[�֘A(�N���X����)
struct ComputeRelation
{
	ComputeRelation();
	~ComputeRelation();

	//�p�C�v���C���X�e�[�g
	std::shared_ptr<KComputePipelineState> computePipelineState;
	//���[�g�V�O�l�`��
	std::unique_ptr<KRootSignature> rootSignature;

	std::unique_ptr<KRootSignature> rootSignature1;
	//�V�F�[�_�[
	std::unique_ptr<KShader> computeShader;

private:
	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	ComputeRelation& operator=(const ComputeRelation&) = delete;
	ComputeRelation(const ComputeRelation&) = delete;
};