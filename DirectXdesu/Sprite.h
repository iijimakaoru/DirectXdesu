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

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

public:
	// ������
	void Init();

	// �`��
	void Draw();

	static SpriteCommon* GetInstance();

private:
	// �萔�o�b�t�@�}�e���A��
	void CreateCBMaterial();

	// �萔�o�b�t�@�g�����X�t�H�[��
	void CreateCBTransform();

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

	// �`��
	void Draw(KTexture* texture);

	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:
	static KGPlin* pipeline;

	SpriteCommon* spriteCommon = nullptr;
};