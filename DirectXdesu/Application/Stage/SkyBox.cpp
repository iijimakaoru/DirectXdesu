#include "SkyBox.h"

SkyBox* SkyBox::Create(KModel* model_, KGPlin* objPipeline_, const float posZ)
{
	// インスタンス生成
	SkyBox* skyBox = new SkyBox();
	if (skyBox == nullptr)
	{
		return nullptr;
	}

	// 初期化
	skyBox->Init(model_, objPipeline_, posZ);

	return skyBox;
}

void SkyBox::Init(KModel* model_, KGPlin* objPipeline_, const float posZ)
{
	// モデル生成
	model = model_;

	// パイプライン生成
	objPipeline = objPipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, objPipeline));
	object3d->SetPos({ object3d->GetPos().x,object3d->GetPos().y,posZ });
	object3d->SetScale({ 600.0f,600.0f,600.0f });
}

void SkyBox::Update(ViewProjection* viewPro_)
{
	object3d->Update(viewPro_);
}

void SkyBox::ObjDraw()
{
	object3d->Draw();
}

void SkyBox::SetPosZ(const float posZ)
{
	object3d->SetPos({ object3d->GetPos().x,object3d->GetPos().y,posZ });
}
