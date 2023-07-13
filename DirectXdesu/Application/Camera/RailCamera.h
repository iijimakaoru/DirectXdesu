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

	// �O�i���邩
	bool isAdvance = true;

	// �O�i�X�s�[�h
	static const float advanceSpeed;

	// �㉺���E���̃J�����̑���(���@�̉��{)
	static const float moveSpeedPlayerMagnification;

	// �ړ����
	KMyMath::Vector2 moveLimitMin = {};
	KMyMath::Vector2 moveLimitMax = {};
};