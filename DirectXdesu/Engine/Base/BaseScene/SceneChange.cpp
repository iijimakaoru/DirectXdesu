#include "SceneChange.h"
#include "Ease.h"
#include "PipelineManager.h"

SceneChange* SceneChange::sceneChange = nullptr;

void SceneChange::Init()
{
	blackTex = TextureManager::GetInstance()->GetTextures("White1x1");

	black.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
}

void SceneChange::Update()
{
	if (isStart)
	{
		isIn = true;
		isEffect = true;
		isStart = false;
	}

	if (isIn)
	{
		inTimer++;

		alpha = MyEase::InCubicFloat(0.0f, 1.0f, (float)inTimer / inTime);

		if (inTimer > inTime)
		{
			isIn = false;
			isChange = true;
			isOut = true;
			inTimer = 0;
		}
	}

	if (isOut)
	{
		outTimer++;

		alpha = MyEase::InCubicFloat(1.0f, 0.0f, (float)outTimer / outTime);

		if (outTimer > outTime)
		{
			isChange = false;
			isOut = false;
			isEffect = false;
			outTimer = 0;
		}
	}
}

void SceneChange::Draw()
{
	black->Draw(blackTex, { 1280 / 2, 720 / 2 }, { 1280 ,720 }, 0.0f, { 0,0,0,alpha });
}

void SceneChange::SceneChangeStart()
{
	isStart = true;
}

void SceneChange::SceneChangeEnd()
{
	isChange = false;
}

SceneChange* SceneChange::GetInstance()
{
    static SceneChange instance;
    return &instance;
}

void SceneChange::Delete()
{
	delete sceneChange;
}

const bool SceneChange::GetIsChange() const
{
	return isChange;
}

const bool SceneChange::GetIsEffect() const
{
	return isEffect;
}
