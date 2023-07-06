#include "Bullet.h"

void Bullet::Init()
{
	// モデル生成
	model = std::make_unique<Cube>();
	model->CreateModel();

	// テクスチャ生成
	tex.CreateTexture("Resources/texture/", "mario.jpg");

	// パイプライン生成
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline = std::make_unique<KGPlin>();
	pipeline->CreatePipelineAll(shader, "Obj");

	// オブジェクト生成
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model.get());
	object3d->transform.scale = { 3.0f,3.0f,3.0f };
}

void Bullet::Set(const KMyMath::Vector3 pos, const KMyMath::Vector3 vec_,
	const KMyMath::Vector3 rot_, const float bulletSpeed)
{
	object3d->transform.pos = pos;
	this->vec = vec_;
	vec = vec.Normalize() * bulletSpeed;
	object3d->transform.rot = rot_;

	lifeTimer = 120;

	isDead = false;
}

void Bullet::Update(ViewProjection* viewPro)
{
	if (!isDead)
	{
		lifeTimer--;
		object3d->transform.pos += vec;

		if (lifeTimer <= 0)
		{
			isDead = true;
		}
	}

	object3d->Update(viewPro);
}

void Bullet::Draw()
{
	if (!isDead)
	{
		object3d->Draw(&tex);
	}
}
