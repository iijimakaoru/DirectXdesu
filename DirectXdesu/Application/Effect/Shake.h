#pragma once
#include "MyMath.h"

class Shake {
public:
	virtual void Update() = 0;

protected:
	float shakeMax_ = 0;
	float shakeMin_ = 0;

	float nowShakeMax_ = 0;
	float nowShakeMin_ = 0;

	float shakeTimer = 0;
	float shakeTime = 0;

	bool isShake = false;
};

class Shake3D : public Shake {
public:
	void Update() override;

	void SetShake(float shakeMax, float shakeMin, float timer);

	const KMyMath::Vector3& GetShakePos() const;

private:
	KMyMath::Vector3 shakePos = {0,0,0};
};

class Shake2D : public Shake {
public:
	void Update() override;

	void SetShake(float shakeMax, float shakeMin, float timer);

	const KMyMath::Vector2& GetShakePos() const;

private:
	KMyMath::Vector2 shakePos = {0, 0};
};