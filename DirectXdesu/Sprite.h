#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"
#include "KMaterial.h"
#include <wrl.h>
#include "KTexture.h"

const int spriteSRVCount = 512;

class SpriteCommon
{
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};
private:
	HRESULT result;

	ID3D12Device* device;

	std::vector<Vertex> vertices;

	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

public:
	// ������
	void Init();

	// �X�V
	void Update();

	// �`��
	void Draw();

	static SpriteCommon* GetInstance();

private:
	

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
	HRESULT result;

	ID3D12Device* device;

	static KGPlin* pipeline;

	SpriteCommon* spriteCommon = nullptr;

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	// �萔�o�b�t�@�̃}�b�v
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	// �萔�o�b�t�@�̃}�b�v
	KConstBufferDataTransform* constMapTransform = nullptr;

public:
	// �ʒu
	DirectX::XMFLOAT2 position = { 0.0f,0.0f };
	// ��]
	float rotation = 0.0f;
};