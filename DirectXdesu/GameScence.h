#pragma once
#include "KDirectXCommon.h"
#include "KInput.h"
#include "KWorldTransform.h"
#include "ViewProjection.h"
#include "KModel.h"
#include "PipelineSet.h"
#include "Sprite.h"
#include "DebugText.h"

class GameScence
{
public:
	GameScence();
	~GameScence();
	void Init();
	void Update();
	void Draw();

private:
	// KDirectCommon
	KDirectXCommon* dx = nullptr;
	// キーボード入力
	KInput* input = nullptr;

	KModel triangle = Triangle();

	KModel cube = Cube();

	KModel line = Line();
};

