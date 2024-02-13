#include "Warning.h"
#include "KWinApp.h"
#include "PipelineManager.h"
#include "ResourceManager.h"

void Warning::Init()
{
	warning.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));

	warningTime = 150;
}

void Warning::Update()
{
	warningTimer++;

	// アルファ値変更
	if (alpha >= max(alpha, 0.5f) || alpha <= min(alpha,0.0f))
	{
		alphaSpeed *= -1;
	}
	alpha += alphaSpeed;

	// 時間で削除
	if (warningTimer >= warningTime)
	{
		isDelete = true;
	}
}

void Warning::Draw()
{
	warning->Draw(
	    ResourceManager::GetInstance()->GetWhite1x1Tex(), {1280 / 2, 720 / 2}, {1280, 720}, 0.0f,
	    {1, 0, 0, alpha});
}

bool Warning::GetIsDelete()
{
	return isDelete;
}

int32_t Warning::GetTime()
{
	return warningTime;
}
