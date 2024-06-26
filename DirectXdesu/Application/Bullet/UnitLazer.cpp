#include "UnitLazer.h"

UnitLazer* UnitLazer::Create(
    KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos_,
    const KMyMath::Vector3& rot_) {
	// インスタンス生成
	UnitLazer* lazer = new UnitLazer();
	if (lazer == nullptr) {
		return nullptr;
	}

	// 初期化
	lazer->Init(model_, pipeline_);
	lazer->Set(pos_, rot_);

	return lazer;
}

void UnitLazer::Init(KModel* model_, KGPlin* pipeline_) {
	// モデル生成
	model = model_;

	// パイプライン生成
	pipeline = pipeline_;

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, pipeline));
	object3d->GetTransform().SetScale({1.0f, 180.0f, 1.0f});

	isDead = true;
}

void UnitLazer::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	object3d->Update(viewPro, cameraPos);
}

void UnitLazer::Draw() {
	if (!isDead) {
		object3d->Draw();
	}
}

void UnitLazer::Set(const KMyMath::Vector3& pos_, const KMyMath::Vector3& rot_) {
	object3d->GetTransform().SetPos(pos_);
	object3d->GetTransform().SetRot(rot_);
}

void UnitLazer::SetIsDead(bool isDead_) { isDead = isDead_; }

void UnitLazer::SetRGB(const KMyMath::Vector3& RGB) { object3d->SetRGB(RGB); }

const bool& UnitLazer::GetIsDead() const { return isDead; }

KMyMath::Vector3 UnitLazer::GetWorldPos() { // ワールド座標格納変数
	KMyMath::Vector3 result;

	// ワールド行列の平行移動成分取得
	result.x = object3d->GetTransform().GetMatWorld().m[3][0];
	result.y = object3d->GetTransform().GetMatWorld().m[3][1];
	result.z = object3d->GetTransform().GetMatWorld().m[3][2];

	return result;
}

void UnitLazer::SetScale(const KMyMath::Vector3& scale) const {
	object3d->GetTransform().SetScale(scale);
}
