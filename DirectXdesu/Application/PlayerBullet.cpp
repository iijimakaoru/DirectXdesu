#include "PlayerBullet.h"

PlayerBullet* PlayerBullet::Create(KModel* model, KGPlin* pipeline_,
	const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed)
{
	// インスタンス生成
	PlayerBullet* playerBullet = new PlayerBullet();
	if (playerBullet == nullptr)
	{
		return nullptr;
	}

	// 初期化
	playerBullet->Init();
	playerBullet->Set(pos, vec_, rot_, bulletSpeed);

	return playerBullet;
}

void PlayerBullet::Init()
{
	// モデル生成
	model = std::make_unique<Cube>();
	model->CreateModel();

	// テクスチャ生成
	texData = TextureManager::Load("Resources/texture/mario.jpg");

	// パイプライン生成
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model.get(),pipeline.get()));
	object3d->transform.scale = { 3.0f,3.0f,3.0f };
}
