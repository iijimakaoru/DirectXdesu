#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"
#include "KMaterial.h"

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
	std::vector<Vertex> vertices;

	// �萔�o�b�t�@
	ID3D12Resource* constBuffMaterial = nullptr;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

public:
	// ������
	void Init();

	// �`��
	void Draw();

	static SpriteCommon* GetInstance();

	// �Q�b�^�[
	D3D12_VERTEX_BUFFER_VIEW& GetVbView();

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
	void Draw();

	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:
	static KGPlin* pipeline;

	SpriteCommon* spriteCommon = nullptr;
};