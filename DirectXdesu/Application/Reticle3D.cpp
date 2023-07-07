#include "Reticle3D.h"

void Reticle3D::Init()
{
	// ���f������
	model = std::make_unique<Cube>();
	model->CreateModel();

	// �e�N�X�`������
	tex.CreateTexture("Resources/texture/", "mario.jpg");

	// �p�C�v���C������
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// �I�u�W�F�N�g����
	object = std::make_unique<KObject3d>();
	object->Initialize();
	object->SetPipeline(pipeline.get());
	object->LoadModel(model.get());
}

void Reticle3D::Update(ViewProjection* viewProjection, KMyMath::Matrix4& m, KMyMath::Vector3 pPos)
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
	object->Update(viewProjection);
}

void Reticle3D::Draw()
{
	object->Draw(&tex);
}
