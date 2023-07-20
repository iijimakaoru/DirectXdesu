#include "Reticle3D.h"

void Reticle3D::Init()
{
	// �I�u�W�F�N�g����
	object = std::make_unique<KObject3d>();
}

void Reticle3D::Update(KMyMath::Matrix4& m, KMyMath::Vector3 pPos)
{
	// ���@����3D�ւ̃��e�B�N���ւ̋���
	const float kDistancePlayerTo3DReticle = 50.0f;

	// ���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z + ����)
	KMyMath::Vector3 offset = { 0,0,1.0f };

	// ���@���烏�[���h�s��̉�]�𔽉f
	offset = KMyMath::Vec3Mat4Mul(offset, m);

	// �x�N�g���̒����𐮂���
	offset = MyMathUtility::MakeNormalize(offset) * kDistancePlayerTo3DReticle;

	// 3D���e�B�N���̍��W�ݒ�
	object->transform.pos = pPos + offset;

	// ���W�X�V
	object->TransUpdate();
}
