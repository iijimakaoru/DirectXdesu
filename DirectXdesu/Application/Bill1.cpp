#include "Bill1.h"

Bill1* Bill1::Create(KModel* model_, KGPlin* objPipeline_, const KMyMath::Vector2& pos)
{
    // �C���X�^���X
    Bill1* instance = new Bill1();
    if (instance == nullptr)
    {
        return nullptr;
    }

    // �������Ăяo��
    instance->Init(model_, objPipeline_, pos);

    return instance;
}

void Bill1::Init(KModel* model_, KGPlin* objPipeline_, const KMyMath::Vector2 pos)
{
    // ���f������
    model = model_;

    // �p�C�v���C������
    objPipeline = objPipeline_;

    // �I�u�W�F�N�g����
    object.reset(KObject3d::Create(model, objPipeline));
    object->transform.pos = { pos.x,-20 ,pos.y };

    isDead = false;
}

void Bill1::Update(ViewProjection* viewPro, const float& cameraZ)
{
    if (object->transform.pos.z <= cameraZ)
    {
        isDead = true;
    }

    object->Update(viewPro);
}

void Bill1::Draw()
{
    object->Draw();
}

const bool Bill1::GetIsDead() const
{
    return isDead;
}
