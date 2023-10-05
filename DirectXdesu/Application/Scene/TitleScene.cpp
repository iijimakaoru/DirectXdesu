#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

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

	// 機体モデル
	model = std::make_unique<MtlObj>("BattleShip");
	model->CreateModel();

	// 天球モデル
	skyDomeModel = std::make_unique<MtlObj>("Hosizora");
	skyDomeModel->CreateModel();

	// フラッシュテクスチャ
	flashTex = TextureManager::Load("Resources/texture/white1x1.png");
}

void TitleScene::Init()
{
	BaseScene::Init();

	// インスタンス
	input = KInput::GetInstance();

	// カメラ読み込み
	camera = std::make_unique<TitleCamera>();
	camera->Init();

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

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model.get(), objPipeline.get()));

	object3d->transform.scale = { 0,0,0 };

	skyDome.reset(KObject3d::Create(skyDomeModel.get(), objPipeline.get()));

	skyDome->transform.scale = { 200,200,200 };

	// フラッシュ
	flash = std::make_unique<Sprite>();
	flash->Init();
	flash->SetPipeline(spritePipeline.get());

	flashAlpha = 0;

	startScene = true;

	startScenePhase = 0;

	phaseTimer = 0;

	phaseTime = 0;

	goGame = false;

	goGamePhase = 0;

	texEaseTimer = 0;

	objEaseTimer = 0;
}

void TitleScene::Update()
{
	if (startScene)
	{
		StartScene();
	}
	else
	{
		if (goGame)
		{
			GoNextScene();
		}
		else
		{
			if (flashAlpha > 0)
			{
				flashAlpha -= 0.1f;
			}

			// 次のシーンへ
			if (!sceneChange->GetIsEffect())
			{
				if (input->IsTrigger(DIK_SPACE) || input->GetPadButtonDown(XINPUT_GAMEPAD_A))
				{
					goGame = true;
					camera->StartSortie();
				}
			}

			object3d->transform.pos = { 0,0,0 };

			object3d->transform.rot = { 0,0,0 };
		}

		if (sceneChange->GetIsChange())
		{
			// シーン切り替え依頼
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
	}

	object3d->Update(camera->GetViewPro());

	skyDome->Update(camera->GetViewPro());

	camera->Update();
}

void TitleScene::ObjDraw()
{
	object3d->Draw();

	skyDome->Draw();
}

void TitleScene::SpriteDraw()
{
	if (!startScene)
	{
		titleName->Draw(titleTex, titlePos, { 1,1 });

		pushA->Draw(pushATex, pushAPos, { 0.5f,0.5f });
	}

	flash->Draw(flashTex, { width / 2,height / 2 }, { width,height }, 0, { 1,1,1,flashAlpha });
}

void TitleScene::Final()
{
	
}

void TitleScene::StartScene()
{
	// シーン遷移待ち
	if (startScenePhase == 0)
	{
		phaseTime = 30;

		phaseTimer++;

		if (phaseTimer > phaseTime)
		{
			startScenePhase++;
			phaseTimer = 0;
		}
	}
	// こっちに飛んできて上に行く
	else if (startScenePhase == 1)
	{
		start = { 0,-30,180 };
		p1 = { 0,-30,20 };
		p2 = { 0,-30,-40 };
		end = { 0,60,-60 };

		phaseTime = 60;

		phaseTimer++;

		// ポイント１の制御点
		KMyMath::Vector3 point1_1 = MyEase::Lerp3D(start, p1, phaseTimer / phaseTime);
		KMyMath::Vector3 point1_2 = MyEase::Lerp3D(p1, end, phaseTimer / phaseTime);
		KMyMath::Vector3 point1 = MyEase::Lerp3D(point1_1, point1_2, phaseTimer / phaseTime);

		// ポイント２の制御点
		KMyMath::Vector3 point2_1 = MyEase::Lerp3D(start, p2, phaseTimer / phaseTime);
		KMyMath::Vector3 point2_2 = MyEase::Lerp3D(p2, end, phaseTimer / phaseTime);
		KMyMath::Vector3 point2 = MyEase::Lerp3D(point2_1, point2_2, phaseTimer / phaseTime);

		object3d->transform.pos = MyEase::Lerp3D(point1, point2, phaseTimer / phaseTime);

		object3d->transform.rot.y = 180;

		object3d->transform.rot.x = MyEase::Lerp(0, -45.0f, phaseTimer / phaseTime);

		if (objEaseTimer < objEaseTime)
		{
			objEaseTimer++;

			object3d->transform.scale =
			{
				MyEase::OutCubicFloat(0, 1, objEaseTimer / objEaseTime),
				MyEase::OutCubicFloat(0, 1, objEaseTimer / objEaseTime),
				MyEase::OutCubicFloat(0, 1, objEaseTimer / objEaseTime)
			};
		}

		if (phaseTimer > phaseTime)
		{
			startScenePhase++;
			phaseTimer = 0;
		}
	}
	// 上から帰ってくる
	else if (startScenePhase == 2)
	{
		start = { 0,60,60 };
		p1 = { 0,40,60 };
		p2 = { 0,0,40 };
		end = { 0,0,0 };

		phaseTime = 60;

		phaseTimer++;

		// ポイント１の制御点
		KMyMath::Vector3 point1_1 = MyEase::Lerp3D(start, p1, phaseTimer / phaseTime);
		KMyMath::Vector3 point1_2 = MyEase::Lerp3D(p1, end, phaseTimer / phaseTime);
		KMyMath::Vector3 point1 = MyEase::Lerp3D(point1_1, point1_2, phaseTimer / phaseTime);

		// ポイント２の制御点
		KMyMath::Vector3 point2_1 = MyEase::Lerp3D(start, p2, phaseTimer / phaseTime);
		KMyMath::Vector3 point2_2 = MyEase::Lerp3D(p2, end, phaseTimer / phaseTime);
		KMyMath::Vector3 point2 = MyEase::Lerp3D(point2_1, point2_2, phaseTimer / phaseTime);

		object3d->transform.pos = MyEase::Lerp3D(point1, point2, phaseTimer / phaseTime);

		object3d->transform.rot.y = 180;

		object3d->transform.rot.x = MyEase::Lerp(45.0f, 0.0f, phaseTimer / phaseTime);

		if (phaseTimer > phaseTime)
		{
			startScenePhase++;
			phaseTimer = 0;
		}
	}
	// スタート画面
	else
	{
		startScene = false;
		camera->StartRound();
		flashAlpha = 1.0f;
	}
}

void TitleScene::GoNextScene()
{
	// カメラ遷移待ち
	if (goGamePhase == 0)
	{
		phaseTime = 45;

		if (texEaseTimer < texEaseTime)
		{
			texEaseTimer++;

			titlePos.y = MyEase::InQuadFloat(height * 1 / 3, (height * 1 / 3) - 300,
				texEaseTimer / texEaseTime);

			pushAPos.y = MyEase::InQuadFloat(height * 2 / 3, (height * 2 / 3) + 300,
				texEaseTimer / texEaseTime);
		}

		if (phaseTimer < phaseTime)
		{
			phaseTimer++;
		}
		else
		{
			goGamePhase++;
			phaseTimer = 0;
		}
	}
	else if (goGamePhase == 1)
	{
		phaseTime = 30;

		if (phaseTimer < phaseTime)
		{
			phaseTimer++;
		}
		else
		{
			goGamePhase++;
			phaseTimer = 0;
		}
	}
	else if (goGamePhase == 2)
	{
		phaseTime = 15;

		start = { 0,0,0 };
		p1 = { 0,0,100 };
		p2 = { 0,0,150 };
		end = { 0,60,180 };

		if (phaseTimer < phaseTime)
		{
			phaseTimer++;

			object3d->transform.scale =
			{
				MyEase::OutCubicFloat(1, 0, phaseTimer / phaseTime),
				MyEase::OutCubicFloat(1, 0, phaseTimer / phaseTime),
				MyEase::OutCubicFloat(1, 0, phaseTimer / phaseTime)
			};

			// ポイント１の制御点
			KMyMath::Vector3 point1_1 = MyEase::Lerp3D(start, p1, phaseTimer / phaseTime);
			KMyMath::Vector3 point1_2 = MyEase::Lerp3D(p1, end, phaseTimer / phaseTime);
			KMyMath::Vector3 point1 = MyEase::Lerp3D(point1_1, point1_2, phaseTimer / phaseTime);

			// ポイント２の制御点
			KMyMath::Vector3 point2_1 = MyEase::Lerp3D(start, p2, phaseTimer / phaseTime);
			KMyMath::Vector3 point2_2 = MyEase::Lerp3D(p2, end, phaseTimer / phaseTime);
			KMyMath::Vector3 point2 = MyEase::Lerp3D(point2_1, point2_2, phaseTimer / phaseTime);

			object3d->transform.pos = MyEase::Lerp3D(point1, point2, phaseTimer / phaseTime);
		}
		else
		{
			goGamePhase++;
			phaseTimer = 0;
		}
	}
	else if (goGamePhase == 3)
	{
		sceneChange->Start();
		goGamePhase++;
	}
}
