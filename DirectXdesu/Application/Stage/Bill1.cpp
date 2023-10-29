#include "Bill1.h"
#include "RailCamera.h"

Bill1* Bill1::Create(KModel* model_, KGPlin* objPipeline_, const KMyMath::Vector2& pos_)
{
    // インスタンス
    Bill1* instance = new Bill1();
    if (instance == nullptr)
    {
        return nullptr;
    }

    // 初期化呼び出し
    instance->Init(model_, objPipeline_, pos_);

    return instance;
}

Bill1::Bill1()
{
}

Bill1::~Bill1()
{
}

void Bill1::Init(KModel* model_, KGPlin* objPipeline_, const KMyMath::Vector2 pos_)
{
    // モデル生成
    model = model_;

    // パイプライン生成
    objPipeline = objPipeline_;

    // オブジェクト生成
    object.reset(KObject3d::Create(model, objPipeline));
    object->SetPos({ pos_.x,-18 ,pos_.y });

    isDead = false;
}

void Bill1::Update(ViewProjection* viewPro_, const float& cameraZ_, const bool& isAdvance_)
{
    if (isAdvance_)
    {
        object->AddSetPos({ 0,0,-RailCamera::GetSpeed() * 2.0f });
    }
    else
    {
        object->AddSetPos({ 0,0,-RailCamera::GetSpeed() });
    }

    if (object->GetPos().z <= cameraZ_)
    {
        isDead = true;
    }

    object->Update(viewPro_);
}

void Bill1::Draw()
{
    object->Draw();
}

const bool Bill1::GetIsDead() const
{
    return isDead;
}
