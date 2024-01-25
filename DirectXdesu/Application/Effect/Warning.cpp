#include "Warning.h"
#include "KWinApp.h"

void Warning::Init()
{
	warningTex = TextureManager::Load("Resources/texture/white1x1.png");

	shader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	pipeline.reset(KGPlin::Create(shader, "Sprite"));

	warning.reset(Sprite::Create(pipeline.get()));

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
	warning->Draw(warningTex,{ 1280 / 2, 720 / 2 }, { 1280 ,720 }, 0.0f, { 1,0,0,alpha });
}

bool Warning::GetIsDelete()
{
	return isDelete;
}

int32_t Warning::GetTime()
{
	return warningTime;
}
