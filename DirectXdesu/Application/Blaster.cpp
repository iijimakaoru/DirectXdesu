#include "Blaster.h"

Blaster* Blaster::Create(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos, const float HP)
{
	// インスタンス生成
	Blaster* blaster = new Blaster();
	if (blaster == nullptr)
	{
		return nullptr;
	}

	// 初期化
	blaster->Init(model_, pipeline_, pos, HP);

	return blaster;
}

void Blaster::Init(KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& initPos, const float HP)
{
	input = KInput::GetInstance();

	// モデル生成
	model = model_;

	// パイプライン生成
	pipeline = pipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->transform.scale = { 10.0f,10.0f,10.0f };
	object3d->transform.pos = initPos;

	// 体力セット
	this->HP = HP;
}

void Blaster::Update(ViewProjection* viewPro)
{
	object3d->Update(viewPro);
}
