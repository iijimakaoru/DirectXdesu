#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"
#include "TitleCamera.h"

#include "SceneManager.h"

#include <imgui.h>

#include "Ease.h"

TitleScene::~TitleScene()
{
	Final();
}

void TitleScene::LoadResources()
{
	// パイプライン
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "Sprite");

	objShader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	objPipeline.reset(KGPlin::Create(objShader, "Obj"));

	// タイトル名テクスチャ
	titleTex = TextureManager::Load("Resources/texture/kariTitle.png");

	// プッシュAテクスチャ
	pushATex = TextureManager::Load("Resources/texture/kariNextScene.png");

	model = std::make_unique<MtlObj>("BattleShip");
	model->CreateModel();
}

void TitleScene::Init()
{
	BaseScene::Init();

	input = KInput::GetInstance();

	camera = std::make_unique<TitleCamera>();

	sceneManager = SceneManager::GetInstance();

	// タイトル名
	titleName = std::make_unique<Sprite>();
	titleName->Init();
	titleName->SetPipeline(spritePipeline.get());

	titlePos = { width / 2 ,height * 1 / 3 };

	// プッシュA
	pushA = std::make_unique<Sprite>();
	pushA->Init();
	pushA->SetPipeline(spritePipeline.get());

	pushAPos = { width / 2, height * 2 / 3 };

	object3d.reset(KObject3d::Create(model.get(), objPipeline.get()));

	goGame = false;

	phase = 0;

	texEaseTimer = 0;

	easeTimer = 0;

	objEaseTimer = 0;
}

void TitleScene::Update()
{
	if (goGame)
	{
		if (phase == 0)
		{
			if (texEaseTimer < texEaseTime)
			{
				texEaseTimer++;

				titlePos.y = MyEase::InQuadFloat(height * 1 / 3, (height * 1 / 3) - 300,
					texEaseTimer / texEaseTime);

				pushAPos.y = MyEase::InQuadFloat(height * 2 / 3, (height * 2 / 3) + 300,
					texEaseTimer / texEaseTime);
			}

			if (easeTimer < easeTime)
			{
				easeTimer++;

				object3d->transform.rot.y = MyEase::InOutCubicFloat(nowAngle, 360, easeTimer / easeTime);
			}
			else
			{
				phase++;
			}
		}
		else if (phase == 1)
		{
			if (objEaseTimer < objEaseTime)
			{
				objEaseTimer++;

				object3d->transform.scale = { 
					MyEase::OutCubicFloat(1, 0, objEaseTimer / objEaseTime),
					MyEase::OutCubicFloat(1, 0, objEaseTimer / objEaseTime),
					MyEase::OutCubicFloat(1, 0, objEaseTimer / objEaseTime) 
				};
				object3d->transform.pos.z = MyEase::OutCubicFloat(0, 150, objEaseTimer / objEaseTime);
			}
			else
			{
				phase++;
			}
		}
		else if (phase == 2)
		{
			sceneChange->Start();
			phase++;
		}
	}
	else
	{
		// 次のシーンへ
		if (!sceneChange->GetIsEffect())
		{
			if (input->IsTrigger(DIK_SPACE) || input->GetPadButtonDown(XINPUT_GAMEPAD_A))
			{
				goGame = true;
			}
		}

		object3d->transform.rot.y += 0.5f;

		if (object3d->transform.rot.y > 360)
		{
			object3d->transform.rot.y = 0;
		}

		nowAngle = object3d->GetRot().y;
	}

	object3d->Update(camera->GetViewPro());

	camera->Update();

	if (sceneChange->GetIsChange())
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::ObjDraw()
{
	object3d->Draw();
}

void TitleScene::SpriteDraw()
{
	titleName->Draw(titleTex, titlePos, { 1,1 });

	pushA->Draw(pushATex, pushAPos, { 1,1 });
}

void TitleScene::Final()
{
	
}
