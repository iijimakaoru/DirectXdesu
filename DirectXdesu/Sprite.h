#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"
#include "KMaterial.h"
#include <wrl.h>
#include "KTexture.h"

class Sprite
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="spriteCommon"></param>
	void Init();

	// �X�V
	void Update();

	// �`��
	void Draw(KTexture* texture, bool isFlipX_ = false, bool isFlipY_ = false,
		DirectX::XMFLOAT2 anchorPoint_ = { 0.5f,0.5f }, DirectX::XMFLOAT2 setSize_ = { 100.0f,100.0f });

	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:
	// �萔�o�b�t�@�}�e���A��
	void CreateCBMaterial();

	// �萔�o�b�t�@�g�����X�t�H�[��
	void CreateCBTransform();

private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	enum VertexNumber
	{
		LB, // ����
		LT, // ����
		RB, // �E��
		RT, // �E��
	};

	struct Info
	{
		// �ʒu
		DirectX::XMFLOAT2 position = { 0.0f,0.0f };
		// ��]
		float rotation = 0.0f;
		// �F
		DirectX::XMFLOAT4 color = { 1,1,1,1 };
		// �\���T�C�Y
		DirectX::XMFLOAT2 size_ = { 100.0f,100.0f };
		// �A���J�[�|�C���g
		DirectX::XMFLOAT2 anchorPoint = { 0.5f,0.5f };
		// ���E�t���b�v
		bool isFlipX = false;
		// �㉺�t���b�v
		bool isFlipY = false;
		// ��\��
		bool isInvisible = false;
	};

	HRESULT result;

	ID3D12Device* device;

	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	// �萔�o�b�t�@�̃}�b�v
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	// �萔�o�b�t�@�̃}�b�v
	KConstBufferDataTransform* constMapTransform = nullptr;

	// ���_�f�[�^
	std::vector<Vertex> vertices;

	// ���_�}�b�v
	Vertex* vertMap = nullptr;

	static KGPlin* pipeline;

public:
	Info info;
};