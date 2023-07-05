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
	WorldTransfom* parent = nullptr;
};

class KObject3d
{
public:
	KObject3d();
	void Initialize();
	void LoadModel(KModel* model);
	void SetPipeline(KGPlin* pipeline_);
	void TransUpdate();
	void MatUpdate(ViewProjection* viewProjection);
	void Update(ViewProjection* viewProjection);
	void Draw();
	void Draw(KTexture* texture);

	void Finalize();

	static void StaticInitialize();
	static KObject3d Create();

private:
	static KGPlin* pipeline;

public:
	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;

	// 3D�I�u�W�F�N�g�̔z��
	WorldTransfom transform;

	HRESULT result;

	KModel* model_ = nullptr;

	KTexture* texture_ = nullptr;
};

