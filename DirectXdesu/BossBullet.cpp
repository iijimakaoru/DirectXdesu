#include "BossBullet.h"

void BossBullet::Init(KModel* model)
{
	// オブジェクトの初期化
	object.Initialize();
	// モデルの読み込み
	model_ = model;
	object.LoadModel(model_);
	// テクスチャ
	texture.CreateTexture("Resources/texture/", "playerColor.png");
}

void BossBullet::SetInfo(const Vector3& position, const Vector3& velocity, const Vector3& scale, const Vector3& rotation, const float lifeTime)
{
	object.transform.pos = position;
	object.transform.scale = scale;

	// 回転量読み込み
	this->rotation = rotation;
	// 向き読み込み
	this->velocity = velocity;
	// 大きさ読み込み
	this->scale = scale;

	lifeTimer = lifeTime;
}

void BossBullet::Update(ViewProjection& viewProjection)
{
	object.transform.pos += velocity;

	object.transform.rot += rotation;

	if (--lifeTimer <= 0)
	{
		isDead = true;
	}

	if (isDead)
	{
		object.transform.rot = { 0,0,0 };
	}

	object.Update(viewProjection);
}

void BossBullet::Draw()
{
	object.Draw(&texture);
}
