#pragma once
#include "KObject3d.h"
#include "KGPlin.h"
#include "KShader.h"

class Reticle3D
{
public:
	void Init();

	void Update(KMyMath::Matrix4& m, KMyMath::Vector3 pPos);

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
};