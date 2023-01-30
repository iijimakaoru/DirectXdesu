#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Vector3.h"
#include "KModel.h"
#include "KTexture.h"
#include "KMaterial.h"
//#include <memory>
#include "DirectionLight.h"
#include "ViewProjection.h"
#include "LightGroup.h"

using namespace DirectX;

// �萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	XMMATRIX mat; // 3D�ϊ��s��
};

struct WorldTransfom {
	// �萔�o�b�t�@(�s��)
	ComPtr<ID3D12Resource> constBuffTransform = {};

	// �萔�o�b�t�@�}�b�v(�s��)
	ConstBufferDataTransform* constMapTransform = {};

	// �A�t�B���ϊ�
	Vector3 scale = { 1,1,1 };
	Vector3 rot = { 0,0,0 };
	Vector3 pos = { 0,0,0 };

	// ���[���h�ϊ��s��
	XMMATRIX matWorld = {};

	// �e�I�u�W�F�N�g�ւ̃|�C���^
	WorldTransfom* parent = nullptr;
};

class KObject3d
{
public:
	KObject3d();
	void Initialize();
	void LoadModel(KModel* model);
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Draw(KTexture* texture);

	/*static void SetLight(DirectionLight* light)
	{
		KObject3d::light = light;
	}*/

	static void SetLightGroup(LightGroup* lightGroup)
	{
		KObject3d::lightGroup = lightGroup;
	}

	static void StaticInitialize();
	static KObject3d Create();

	Vector3 rotResult;

	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;

	// 3D�I�u�W�F�N�g�̔z��
	WorldTransfom transform;

	HRESULT result;

	KModel* model_ = nullptr;

	KTexture* texture_ = nullptr;

	// �}�e���A��
	KMaterial* material = nullptr;

	//static DirectionLight* light;
	static LightGroup* lightGroup;
};

