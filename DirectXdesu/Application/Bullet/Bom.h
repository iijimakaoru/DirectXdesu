#pragma once
#include "AudioManager.h"
#include "Camera.h"
#include "Collider.h"
#include "KGPlin.h"
#include "KModel.h"
#include "KObject3d.h"
#include "KShader.h"
#include "TextureManager.h"

class Bom : public Collider {
public:
	static Bom* Create(
	    KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& pos_,
	    const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed_);

	void Init(KModel* model_, KGPlin* pipeline_);

	void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos);

	void Draw();

	void
	    Set(const KMyMath::Vector3& pos_, const KMyMath::Vector3& vec_,
	        const KMyMath::Vector3& rot_, const float bulletSpeed_);

	void OnCollision(Collider* collider) override;

	KMyMath::Vector3 GetWorldPosition() override;

	void SetIsDead(bool isDead_);

	const bool& GetIsDead() const;

	const bool& GetIsExp() const;

	const float& GetBomsPower() const;

	const float& GetExpPower() const;

	void SetBomHit(bool isBomHit_);

	const bool& GetBomHit() const;

	const KMyMath::Vector3 GetWorldPos() const;

private:
	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	KModel* model;

	// パイプライン
	KGPlin* pipeline;

	// スピード
	float speed = 1.0f;

	bool isExp = false;

	bool isDead = false;

	KMyMath::Vector3 vec;

	float lifeTimer = 0.0f;

	const float bomPower = 10;
	const float expPower = 20;

	// ボス限定
	bool isBomHit = false;

	AudioManager* audioManager = nullptr;
};
