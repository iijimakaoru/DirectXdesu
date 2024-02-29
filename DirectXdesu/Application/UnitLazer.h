#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "KModel.h"
#include "KObject3d.h"
#include "KShader.h"
#include "TextureManager.h"

class UnitLazer {
public:
	static UnitLazer* Create(
	    KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos_,
	    const KMyMath::Vector3& rot_);

	void Init(KModel* model_, KGPlin* pipeline_);

	void Update(ViewProjection* viewPro_);

	void Draw();

	void Set(const KMyMath::Vector3& pos_, const KMyMath::Vector3& rot_);

	void SetIsDead(bool isDead_);

	const bool& GetIsDead() const;

	// ワールドポジションゲッター
	KMyMath::Vector3 GetWorldPos();

	void SetScale(const KMyMath::Vector3& scale) const;

private:
	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	KModel* model;

	// パイプライン
	KGPlin* pipeline;

	bool isDead;
};
