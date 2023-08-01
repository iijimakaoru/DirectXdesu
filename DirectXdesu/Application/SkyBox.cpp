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
	object3d->transform.pos.z = posZ;
	object3d->transform.scale = { 600.0f,600.0f,600.0f };
}

void SkyBox::Update(ViewProjection* viewPro)
{
	object3d->Update(viewPro);
}

void SkyBox::ObjDraw()
{
	object3d->Draw();
}

void SkyBox::SetPosZ(const float posZ)
{
	object3d->transform.pos.z = posZ;
}
