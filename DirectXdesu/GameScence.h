#pragma once
#include "KDirectXCommon.h"
#include "KInput.h"
#include "KWorldTransform.h"
#include "ViewProjection.h"
#include "KModel.h"
#include "PipelineSet.h"
#include "Sprite.h"
#include "DebugText.h"
#include "KTexture.h"
#include "Sound.h"

const int ObjectNum = 2;
const int LineNum = 6;

class GameScence
{
public:
	GameScence();
	~GameScence();
	void Init(ID3D12Device& dev, int window_width, int window_height);
	void Update();
	void Draw(ID3D12Device& dev, ID3D12GraphicsCommandList* cmdList);

private:
	// KDirectCommon
	KDirectXCommon* dx = nullptr;
	// �L�[�{�[�h����
	KInput* input = nullptr;

	KModel triangle = Triangle();

	KModel cube = Cube();

	KModel line = Line();

	// 3D�I�u�W�F�N�g
	KWorldTransform* object3d[ObjectNum];

	// �X�v���C�g
	std::unique_ptr<Sprite> sprite;

	// �r���[�v���W�F�N�V����
	ViewProjection* viewProjection;

	Vector3 center = { 0,0,1 };

	float rSpeed = -0.02f;
	float gSpeed = 0.02f;
	float bSpeed = -0.02f;
	float aSpeed = -0.02f;

	std::unique_ptr<Sound> sound;

	SpriteCommon spriteCommon;

	SpriteInfo sprites[2];

	std::unique_ptr<DebugText> debugtext;
};

