#pragma once
#include "KDirectXCommon.h"
#include "KInput.h"
#include "KObject3d.h"
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
	GameScence(){};
	~GameScence(){};
	void Init();
	void Update();
	void Draw();

private:


};

