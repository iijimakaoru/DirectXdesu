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
	mesiTex = TextureManager::Load("Resources/texture/MESI.png");
	shooterTex = TextureManager::Load("Resources/texture/SHOOTER.png");
	moldTex = TextureManager::Load("Resources/texture/BattleShipMold.png");

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

	mesi = std::make_unique<Sprite>();
	mesi->Init();
	mesi->SetPipeline(spritePipeline.get());
	
	shooter = std::make_unique<Sprite>();
	shooter->Init();
	shooter->SetPipeline(spritePipeline.get());
	shooterScale = { 1.0f,1.0f };

	mold = std::make_unique<Sprite>();
	mold->Init();
	mold->SetPipeline(spritePipeline.get());
	moldScale = { 1.6f,1.6f };

	// プッシュA
	pushA = std::make_unique<Sprite>();
	pushA->Init();
	pushA->SetPipeline(spritePipeline.get());
	pushAPos = { width / 2, height * 4 / 5 };

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
		mold->Draw(moldTex, titlePos + moldPos, moldScale);

		mesi->Draw(mesiTex, titlePos + mesiPos, mesiScale);

		shooter->Draw(shooterTex, titlePos + shooterPos, shooterScale);

		pushA->Draw(pushATex, pushAPos, { 0.75f,0.75f });
	}
	else
	{
		mold->Draw(moldTex, titlePos + moldPos, moldScale);

		shooter->Draw(shooterTex, titlePos + shooterPos, shooterScale);

		mesi->Draw(mesiTex, titlePos + mesiPos, mesiScale);
	}

	flash->Draw(flashTex, { width / 2,height / 2 }, { width,height }, 0, { 1,1,1,flashAlpha });
}

void TitleScene::Final()
{
	
}

void TitleScene::StartScene()
{
	// タイトル導入スキップ
	if (startScenePhase < 4)
	{
		if (input->GetPadButtonDown(XINPUT_GAMEPAD_A))
		{
			startScenePhase = 4;
		}
	}

	if (isTitle)
	{
		TitleCall();
	}

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
		start = { 0,-20,190 };
		p1 = { 0,-20,20 };
		p2 = { 0,-30,-30 };
		end = { 0,50,-30 };

		phaseTime = 90;

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
		start = { 0,80,80 };
		p1 = { 0,50,80 };
		p2 = { 0,0,60 };
		end = { 0,-3,30 };

		phaseTime = 45;

		phaseTimer++;

		// ベジエ曲線
		// ポイント１の制御点
		KMyMath::Vector3 point1_1 = MyEase::Lerp3D(start, p1, phaseTimer / phaseTime);
		KMyMath::Vector3 point1_2 = MyEase::Lerp3D(p1, end, phaseTimer / phaseTime);
		KMyMath::Vector3 point1   = MyEase::Lerp3D(point1_1, point1_2, phaseTimer / phaseTime);

		// ポイント２の制御点
		KMyMath::Vector3 point2_1 = MyEase::Lerp3D(start, p2, phaseTimer / phaseTime);
		KMyMath::Vector3 point2_2 = MyEase::Lerp3D(p2, end, phaseTimer / phaseTime);
		KMyMath::Vector3 point2   = MyEase::Lerp3D(point2_1, point2_2, phaseTimer / phaseTime);

		object3d->transform.pos = MyEase::Lerp3D(point1, point2, phaseTimer / phaseTime);

		object3d->transform.rot.y = 180;

		object3d->transform.rot.x = MyEase::Lerp(45.0f, 0.0f, phaseTimer / phaseTime);

		isTitle = true;

		if (phaseTimer > phaseTime)
		{
			startScenePhase++;
			phaseTimer = 0;
		}
	}
	// 余韻
	else if (startScenePhase == 3)
	{
		phaseTime = 30;

		phaseTimer++;

		object3d->transform.pos.z = MyEase::OutCubicFloat(30, 0, phaseTimer / phaseTime);

		object3d->transform.rot.z = MyEase::OutCubicFloat(0, 360, phaseTimer / phaseTime);

		if (phaseTimer > phaseTime)
		{
			startScenePhase++;
			phaseTimer = 0;
		}
	}
	else if (startScenePhase == 4)
	{
		// フラッシュ！
		flashAlpha = 1.0f;
		// オブジェクト情報タイトルどうりに
		object3d->transform.pos = { 0,0,0 };
		object3d->transform.rot = { 0,0,0 };
		// タイトル位置
		shooterPos.x = 200;
		moldPos.x = 0;
		mesiPos = { -200,-30 };
		mesiScale = { 1.0f,1.0f };

		startScenePhase++;
	}
	// スタート画面
	else
	{
		startScene = false;
		camera->StartRound();
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

			titlePos.y = MyEase::InQuadFloat(height * 1 / 3, (height * 1 / 3) - 500,
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

void TitleScene::TitleCall()
{
	if (titlePhase == 0)
	{
		titlePhaseTime = 30;

		if (titlePhaseTimer < titlePhaseTime)
		{
			titlePhaseTimer++;

			shooterPos.x = MyEase::OutCubicFloat(1200, 200, titlePhaseTimer / titlePhaseTime);
			moldPos.x = MyEase::OutCubicFloat(-1200, 0, titlePhaseTimer / titlePhaseTime);
		}
		else
		{
			titlePhase++;
			titlePhaseTimer = 0;
		}
	}
	else if (titlePhase == 1)
	{
		titlePhaseTime = 45;

		if (titlePhaseTimer < titlePhaseTime)
		{
			titlePhaseTimer++;

			mesiPos = MyEase::InCubicVec2({ 0,0 }, { -200,-30 }, titlePhaseTimer / titlePhaseTime);
			mesiScale = MyEase::InCubicVec2({ 2.5f,2.5f }, { 1.0f,1.0f }, titlePhaseTimer / titlePhaseTime);
		}
		else
		{
			titlePhase++;
			titlePhaseTimer = 0;
		}
	}
	else
	{
		isTitle = false;
	}
}
