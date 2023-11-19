#include "SkyBox.h"

SkyBox* SkyBox::Create(KModel* model_, KGPlin* objPipeline_, const float posZ_)
{
	// インスタンス生成
	SkyBox* skyBox = new SkyBox();
	if (skyBox == nullptr)
	{
		return nullptr;
	}

	// 初期化
	skyBox->Init(model_, objPipeline_, posZ_);

	return skyBox;
}

void SkyBox::Init(KModel* model_, KGPlin* objPipeline_, const float posZ_)
{
	// モデル生成
	model = model_;

	// パイプライン生成
	objPipeline = objPipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, objPipeline));
	object3d->SetPos({ object3d->GetPos().x,-20.0f,posZ_ });
	object3d->SetScale({ 1.0f,1.0f,1.0f });
}

void SkyBox::Update(ViewProjection* viewPro_)
{
	object3d->Update(viewPro_);
}

void SkyBox::ObjDraw()
{
	object3d->Draw();
}

void SkyBox::SetPosZ(const float posZ_)
{
	object3d->SetPos({ object3d->GetPos().x,object3d->GetPos().y,posZ_ });
}
