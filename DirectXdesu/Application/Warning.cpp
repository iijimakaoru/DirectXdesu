#include "Warning.h"
#include "KWinApp.h"

void Warning::Init()
{
	warningTex = TextureManager::Load("Resources/texture/white1x1.png");

	shader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");

	pipeline = std::make_unique<KGPlin>();
	pipeline->CreatePipelineAll(shader, "Sprite");

	warning = std::make_unique<Sprite>();
	warning->Init();
	warning->SetPipeline(pipeline.get());

	warningTime = 300;
}

void Warning::Update()
{
	warningTimer++;

	// �A���t�@�l�ύX
	if (alpha >= max(alpha, 0.5f) || alpha <= min(alpha,0.0f))
	{
		alphaSpeed *= -1;
	}
	alpha += alphaSpeed;

	// ���Ԃō폜
	if (warningTimer >= warningTime)
	{
		isDelete = true;
	}
}

void Warning::Draw()
{
	warning->Draw(warningTex,{ 1280 / 2, 720 / 2 }, { 1280 ,720 }, 0.0f, { 1,0,0,alpha });
}