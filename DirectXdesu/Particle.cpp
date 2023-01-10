#include "Particle.h"
#include "Ease.h"

void Particle::Init(KModel* model)
{
	// オブジェクトの初期化
	object.Initialize();
	// モデルの読み込み
	model_ = model;
	object.LoadModel(model_);
	// テクスチャ
	texture.CreateTexture("Resources/texture/", "damageColor.png");
}

void Particle::SetInfo(const Vector3& pos, const Vector3& velocity, const Vector3& scale, const Vector3& rot, const float kLifeTime)
{
	object.transform.pos = pos;
	object.transform.scale = { 0,0,0 };

	// 回転量読み込み
	this->rotation = rot;
	// 向き読み込み
	this->velocity = velocity;
	// 大きさ読み込み
	this->scale = scale;

	maxLifeTimer = kLifeTime;
	lifeTimer = maxLifeTimer;
}

void Particle::Update(ViewProjection& view)
{
	if (isExp)
	{
		object.transform.scale.x = EaseIn(object.transform.scale.x, scale.x, 1.0f / lifeTimer, 0.5f);
		object.transform.scale.y = EaseIn(object.transform.scale.y, scale.y, 1.0f / lifeTimer, 0.5f);
		object.transform.scale.z = EaseIn(object.transform.scale.z, scale.z, 1.0f / lifeTimer, 0.5f);
	}
	else
	{
		object.transform.scale.x = EaseIn(scale.x, 0, 1.0f / lifeTimer, 0.5f);
		object.transform.scale.y = EaseIn(scale.y, 0, 1.0f / lifeTimer, 0.5f);
		object.transform.scale.z = EaseIn(scale.z, 0, 1.0f / lifeTimer, 0.5f);
	}

	if (lifeTimer >= maxLifeTimer / 2)
	{
		isExp = true;
	}
	else
	{
		isExp = false;
	}

	// 移動
	object.transform.pos += velocity;

	// 回転
	object.transform.rot += rotation;

	// アプデ
	object.Update(view);

	// 消える処理
	if (--lifeTimer <= 0)
	{
		isDead = true;
	}
}

void Particle::Draw()
{
	object.Draw(&texture);
}
