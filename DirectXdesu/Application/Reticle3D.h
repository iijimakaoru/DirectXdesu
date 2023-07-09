#pragma once
#include "KObject3d.h"
#include "KGPlin.h"
#include "KShader.h"

class Reticle3D
{
public:
	void Init();

	void Update(ViewProjection* viewProjection, KMyMath::Matrix4& m, KMyMath::Vector3 pPos);

	void Draw();

	KMyMath::Vector3 GetWorldPos()
	{
		// ���[���h���W�i�[�ϐ�
		KMyMath::Vector3 result;

		// ���[���h�s��̕��s�ړ������擾
		result.x = object->transform.matWorld.m[3][0];
		result.y = object->transform.matWorld.m[3][1];
		result.z = object->transform.matWorld.m[3][2];

		return result;
	}

private:
	std::unique_ptr<KObject3d> object = nullptr;

	std::unique_ptr<KGPlin> pipeline = nullptr;

	KShader shader;

	KTexture tex;

	std::unique_ptr<KModel> model = nullptr;
};