#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Vector3.h"
#include "KModel.h"
#include "KTexture.h"
#include "ViewProjection.h"
#include "KGPlin.h"

using namespace DirectX;

struct ConstBufferDataB1
{
	KMyMath::Vector3 ambient;
	float pad1;
	KMyMath::Vector3 diffuse;
	float pad2;
	KMyMath::Vector3 specular;
	float alpha;
};

struct ConstBufferDataB0
{
	KMyMath::Matrix4 mat;
};

struct WorldTransfom {
	// �萔�o�b�t�@(�s��)
	ID3D12Resource* constBuffTransform = {};

	// �萔�o�b�t�@�}�b�v(�s��)
	ConstBufferDataB0* constMapTransform = {};

	// �A�t�B���ϊ�
	KMyMath::Vector3 scale = { 1,1,1 };
	KMyMath::Vector3 rot = { 0,0,0 };
	KMyMath::Vector3 pos = { 0,0,0 };

	// ���[���h�ϊ��s��
	KMyMath::Matrix4 matWorld = {};

	// �e�I�u�W�F�N�g�ւ̃|�C���^
	const WorldTransfom* parent = nullptr;
};

class KObject3d
{
public:
	KObject3d();
	// ������
	void Initialize();
	/// <summary>
	/// ���f���ǂݍ���(�Z�b�^�[)
	/// </summary>
	/// <param name="model"></param>
	void LoadModel(KModel* model);
	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);
	/// <summary>
	/// �g�����X�t�H�[���X�V
	/// </summary>
	void TransUpdate();
	/// <summary>
	/// �s��X�V
	/// </summary>
	/// <param name="viewProjection"></param>
	void MatUpdate(ViewProjection* viewProjection);
	/// <summary>
	/// ��̕���
	/// </summary>
	/// <param name="viewProjection"></param>
	void Update(ViewProjection* viewProjection);
	/// <summary>
	/// �f�t�H���g�e�N�X�`���`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �e�N�X�`���؂�ւ��`��
	/// </summary>
	/// <param name="texture"></param>
	void Draw(KTexture* texture);

	/// <summary>
	/// �e�q�Z�b�^�[
	/// </summary>
	/// <param name="parent_"></param>
	void SetParent(WorldTransfom* parent_);

	void Finalize();

	static void StaticInitialize();
	static KObject3d Create();

private:
	KGPlin* pipeline;

public:
	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;

	// 3D�I�u�W�F�N�g�̔z��
	WorldTransfom transform;

	HRESULT result;

	KModel* model_ = nullptr;

	KTexture* texture_ = nullptr;
};

