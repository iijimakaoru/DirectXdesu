#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

TitleScene::~TitleScene()
{
	Final();
}

void TitleScene::LoadResources()
{
	tex1.CreateTexture("Resources/texture/", "mario.jpg");
}

void TitleScene::Init()
{
	input = KInput::GetInstance();

	// Fbx
	fbxShader.Init(L"Resources/Shader/FBXVS.hlsl", L"Resources/Shader/FBXPS.hlsl");
	fbxPipeline = std::make_unique<KGPlin>();
	fbxPipeline->CreatePipelineAll(fbxShader, false, false, false, true);

	// Sprite
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, false, true);

#pragma region Fbxテスト
	fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	object1 = new FbxObject3D;
	object1->Init();
	object1->SetPipline(fbxPipeline.get());
	object1->SetModel(fbxModel1);
	object1->PlayAnimation();
#pragma endregion

	camera = std::make_unique<DebugCamera>();

	// スプライト
	sprite = new Sprite();
	sprite->Init();
	sprite->SetPipeline(spritePipeline.get());

	// ポストエフェクトテスト
	postEffect = std::make_unique<PostEffect>();
	postEffect->Init();
	postEffect->SetPipeline(spritePipeline.get());

	sceneManager = SceneManager::GetInstance();
}

void TitleScene::Update()
{
	ImGui::Text("Sprite");
	ImGui::Text("pos: (%.2f,%.2f)", spritePos.x, spritePos.y);
	ImGui::SliderFloat("SpritePosX", &spritePos.x, 0.0f, 100.0f);
	ImGui::SliderFloat("SpritePosY", &spritePos.y, 0.0f, 100.0f);
	ImGui::Text("size: (%.2f,%.2f)", spriteSize.x, spriteSize.y);
	ImGui::SliderFloat("SpriteSizeX", &spriteSize.x, 0.0f, 100.0f);
	ImGui::SliderFloat("SpriteSizeY", &spriteSize.y, 0.0f, 100.0f);
	ImGui::Text("rot: (%.2f)", spriteRot);
	ImGui::SliderFloat("SpriteRot", &spriteRot, 0.0f, 180.0f);
	ImGui::Text("color: (%.2f,%.2f,%.2f,%.2f)", spriteColor.x, spriteColor.y, spriteColor.z, spriteColor.w);
	ImGui::SliderFloat("SpriteColorR", &spriteColor.x, 0.0f, 1.0f);
	ImGui::SliderFloat("SpriteColorG", &spriteColor.y, 0.0f, 1.0f);
	ImGui::SliderFloat("SpriteColorB", &spriteColor.z, 0.0f, 1.0f);
	ImGui::SliderFloat("SpriteColorA", &spriteColor.w, 0.0f, 1.0f);
	ImGui::Text("Flip");
	if (ImGui::Button("FlipX"))
	{
		if (spriteFlipX)
		{
			spriteFlipX = false;
		}
		else
		{
			spriteFlipX = true;
		}
	}
	if (ImGui::Button("FlipY"))
	{
		if (spriteFlipY)
		{
			spriteFlipY = false;
		}
		else
		{
			spriteFlipY = true;
		}
	}

	// Fbx
	object1->Update(camera->viewProjection);

	camera->Update();

	// 次のシーンへ
	if (input->IsTrigger(DIK_SPACE))
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	object1->Draw();

	sprite->Draw(&tex1, spritePos, spriteSize, spriteRot, spriteColor, spriteFlipX, spriteFlipY);

	//postEffect->Draw(&tex1, { 100,100 }, { 200,200 });
}

void TitleScene::Final()
{
	delete object1;
	delete fbxModel1;
}
