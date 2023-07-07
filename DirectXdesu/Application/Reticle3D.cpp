#include "Reticle3D.h"

void Reticle3D::Init()
{
	// モデル生成
	model = std::make_unique<Cube>();
	model->CreateModel();

	// テクスチャ生成
	tex.CreateTexture("Resources/texture/", "mario.jpg");

	// パイプライン生成
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// オブジェクト生成
	object = std::make_unique<KObject3d>();
	object->Initialize();
	object->SetPipeline(pipeline.get());
	object->LoadModel(model.get());
}

void Reticle3D::Update(ViewProjection* viewProjection, KMyMath::Matrix4& m, KMyMath::Vector3 pPos)
{
	// 自機から3Dへのレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;

	// 自機から3Dレティクルへのオフセット(Z + 向き)
	KMyMath::Vector3 offset = { 0,0,1.0f };

	// 自機からワールド行列の回転を反映
	offset = KMyMath::Vec3Mat4Mul(offset, m);

	// ベクトルの長さを整える
	offset = MyMathUtility::MakeNormalize(offset) * kDistancePlayerTo3DReticle;

	// 3Dレティクルの座標設定
	object->transform.pos = pPos + offset;

	// 座標更新
	object->Update(viewProjection);
}

void Reticle3D::Draw()
{
	object->Draw(&tex);
}
