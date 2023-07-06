#pragma once
#include "KTexture.h"
#include "MyMath.h"
#include "KGPlin.h"
#include "KWinApp.h"

class PostEffect
{
public:// �ÓI�����o�֐�
	//�@�ÓI������
	static void StaticInit();

public:
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �`����
	/// </summary>
	void DrawCommand();

	// �V�[���`��O����
	void PreDrawScene();

	// �V�[���`��㏈��
	void PostDrawScene();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:// �N���X���ł����g��Ȃ��֐�
	// �萔�o�b�t�@�}�e���A��
	void CreateCBMaterial();

	// ���_����
	void CreateVertex();

	// �萔�o�b�t�@�g�����X�t�H�[��
	void CreateCBTransform();

	// �e�N�X�`���o�b�t�@
	void CreateTextureBuff();

	// �[�x�o�b�t�@
	void CreateDepthBuff();

	// DSV�p�f�X�N���v�^�q�[�v
	void CreateDSVDescHeap();

	// RTV�p�f�X�N���v�^�q�[�v
	void CreateRTVDescHeap();

private:
	struct Vertex
	{
		KMyMath::Vector3 pos;
		KMyMath::Vector2 uv;
	};

	enum VertexNumber
	{
		LB, // ����
		LT, // ����
		RB, // �E��
		RT, // �E��
	};

	HRESULT result;

	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	// �萔�o�b�t�@�̃}�b�v
	KMyMath::Vector4* constMapMaterial = nullptr;

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	// �萔�o�b�t�@�̃}�b�v
	KMyMath::Matrix4* constMapTransform = nullptr;

	// �e�N�X�`���o�b�t�@
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,2> texBuff = { nullptr };

	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	// �[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff = nullptr;

	// RTV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	// DSV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	Vertex vertices_[4];

private:// �ÓI�����o�ϐ�
	// �f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D12Device> device;

	// �R�}���h���X�g
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;

	// �p�C�v���C��
	static KGPlin* pipeline;

	// �E�B���h�E
	static KWinApp* window;

	// ��ʃN���A�J���[
	static const float clearColor[4];
};

