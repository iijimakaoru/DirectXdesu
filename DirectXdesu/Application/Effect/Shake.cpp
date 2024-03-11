#include "Shake.h"
#include "Ease.h"

void Shake3D::Update() {
	if (isShake) {
		if (shakeTimer < shakeTime) {
			shakeTimer++;

			nowShakeMax_ = MyEase::Lerp(shakeMax_, 0, shakeTimer / shakeTime);
			nowShakeMin_ = MyEase::Lerp(shakeMin_, 0, shakeTimer / shakeTime);

			shakePos = {
			    MyMathUtility::GetRandF(nowShakeMin_, nowShakeMax_),
			    MyMathUtility::GetRandF(nowShakeMin_, nowShakeMax_),
			    MyMathUtility::GetRandF(nowShakeMin_, nowShakeMax_)};
		} else {
			isShake = false;
		}
	} else {
		shakePos = {0, 0, 0};
	}
}

void Shake3D::SetShake(float shakeMax, float shakeMin, float timer) {
	shakeMax_ = shakeMax;
	shakeMin_ = shakeMin;

	shakeTime = timer;
	shakeTimer = 0;

	isShake = true;
}

const KMyMath::Vector3& Shake3D::GetShakePos() const { return shakePos; }

void Shake2D::Update() {
	if (isShake) {
		if (shakeTimer < shakeTime) {
			shakeTimer++;

			nowShakeMax_ = MyEase::Lerp(shakeMax_, 0, shakeTimer / shakeTime);
			nowShakeMin_ = MyEase::Lerp(shakeMin_, 0, shakeTimer / shakeTime);

			shakePos = {
			    MyMathUtility::GetRandF(nowShakeMin_, nowShakeMax_),
			    MyMathUtility::GetRandF(nowShakeMin_, nowShakeMax_)};
		} else {
			isShake = false;
		}
	} else {
		shakePos = {0, 0};
	}
}

void Shake2D::SetShake(float shakeMax, float shakeMin, float timer) {
	shakeMax_ = shakeMax;
	shakeMin_ = shakeMin;

	shakeTime = timer;
	shakeTimer = 0;

	isShake = true;
}

const KMyMath::Vector2& Shake2D::GetShakePos() const { return shakePos; }
