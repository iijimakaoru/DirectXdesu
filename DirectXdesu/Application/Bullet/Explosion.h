#pragma once
#include "AudioManager.h"
#include "Collider.h"
#include "KObject3d.h"

class Explosion : public Collider {
public:
	static Explosion* Create(const KMyMath::Vector3& pos, const float time);

	void Init();

	void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos);

	void Draw();

	void Set(const KMyMath::Vector3& pos, const float time);

	void SetIsDead(bool isDead);

	const bool& GetIsDead() const;

	void OnCollision(Collider* collider) override;

	KMyMath::Vector3 GetWorldPosition() override;

private:
	std::unique_ptr<KObject3d> object3d_;

	float expTimer_ = 0.0f;
	float expTime_ = 30.0f;

	// ボス限定
	bool isBomHit_ = false;

	KMyMath::Vector3 expRGB_ = {251, 100, 24};
	float expAlpha_ = 255.0f;

	bool isDead_ = false;

	AudioManager* audioManager_ = nullptr;
};
