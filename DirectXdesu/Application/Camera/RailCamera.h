#pragma once
#include "Camera.h"
#include "KObject3d.h"

class Player;

class RailCamera : public Camera
{
public:
	RailCamera() 
	{
		Init();
	};

	void Init() override;

	void Update(Player* player);

	void SetRadian(float angle_)
	{
		angle = angle_;
	}

	void Move();

	void SetRot(const KMyMath::Vector3& playersRot);

private:
	float angle = 0;

	// 前進するか
	bool isAdvance = true;

	// 前進スピード
	static const float advanceSpeed;

	// 上下左右時のカメラの速さ(自機の何倍)
	static const float moveSpeedPlayerMagnification;

	// 移動上限
	KMyMath::Vector2 moveLimitMin = {};
	KMyMath::Vector2 moveLimitMax = {};
};