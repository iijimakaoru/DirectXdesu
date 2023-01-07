#pragma once
#include "KObject3d.h"
#include "KModel.h"
#include "Vector2.h"
#include "KTexture.h"
#include "ViewProjection.h"

class Player
{
public:
	void Init(KModel* model);
	void Update(ViewProjection& viewProjection);
	void Draw();

	// �I�u�W�F�N�g
	KObject3d object;

	// ���f��
	KModel* model_;

	// ���x
	float speed;

	// 
	bool isJump;
	float jumpPower;
	const int jumpPowerMax = 5;

	Vector2 stick;

	// �e�N�X�`��
	KTexture texture_;

	ViewProjection view;

	static Player* nowPlayer;
};

