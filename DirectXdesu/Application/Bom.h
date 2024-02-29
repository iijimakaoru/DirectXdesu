#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "KModel.h"
#include "KObject3d.h"
#include "KShader.h"
#include "TextureManager.h"

class Bom {
public:
	static Bom* Create(
	    KModel* model_, KModel* expModel_, KGPlin* pipeline_, const KMyMath::Vector3& pos_,
	    const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed_);

	void Init(KModel* model_, KModel* expModel_, KGPlin* pipeline_);

	void Update(ViewProjection* viewPro_);

	void Draw();

	void
	    Set(const KMyMath::Vector3& pos_, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
	        const float bulletSpeed_);

	void OnCollision();

	void SetIsDead(bool isDead_);

	const bool& GetIsDead() const;

	const bool& GetIsExp() const;

	// ワールドポジションゲッター
	KMyMath::Vector3 GetWorldPos();

	const float& GetBomsPower() const;

	const float& GetExpPower() const;

	void SetBomHit(bool isBomHit_);

	const bool& GetBomHit() const;

private:
	// オブジェクト
	std::unique_ptr<KObject3d> object3d;
	std::unique_ptr<KObject3d> expObject;

	// モデル
	KModel* model;

	// 爆発モデル
	KModel* expModel;

	// パイプライン
	KGPlin* pipeline;

	// スピード
	float speed = 1.0f;

	bool isExp = false;

	bool isDead = false;

	KMyMath::Vector3 vec;

	float lifeTimer = 0.0f;

	float expTimer = 0.0f;
	float expTime = 30.0f;

	const float bomPower = 10;
	const float expPower = 20;

	// ボス限定
	bool isBomHit = false;

	float expAlpha = 1.0f;
};
