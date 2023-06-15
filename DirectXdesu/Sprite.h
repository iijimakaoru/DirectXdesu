#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"
#include "KMaterial.h"
#include <wrl.h>
#include "KTexture.h"

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

class SpriteCommon
{
private:
	
private:
	HRESULT result;

	ID3D12Device* device;

public:
	// ������
	void Init();

	// �X�V
	void Update();

	// �`��
	void Draw();

	static SpriteCommon* GetInstance();

	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:
	KGPlin* pipeline;

private:
	SpriteCommon() = default;
	~SpriteCommon() = default;

	SpriteCommon(const SpriteCommon&) = default;
	const SpriteCommon& operator=(const SpriteCommon&) = delete;
};

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
	void Draw(KTexture* texture);

private:
	// �萔�o�b�t�@�}�e���A��
	void CreateCBMaterial();

	// �萔�o�b�t�@�g�����X�t�H�[��
	void CreateCBTransform();

private:
	enum VertexNumber
	{
		LB, // ����
		LT, // ����
		RB, // �E��
		RT, // �E��
	};

	HRESULT result;

	ID3D12Device* device;

	SpriteCommon* spriteCommon = nullptr;

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

	Vertex* vertMap = nullptr;

public:
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
};