#include "GameScence.h"
#include "KInput.h"

#include "ParticleManager.h"
#include "MyMath.h"

#include "DebugCamera.h"
#include "GameCamera.h"

GameScence::~GameScence()
{
	delete camera;
	sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);
};

void GameScence::LoadResources()
{
	// モデル
	triangle = std::make_unique<Triangle>();
	triangle->CreateModel();
	cube = std::make_unique<Cube>();
	cube->CreateModel();
	hoge = std::make_unique<MtlObj>("hoge");
	hoge->CreateModel();
	SkyBox = std::make_unique<MtlObj>("boxSky");
	SkyBox->CreateModel();

	// テクスチャ
	mario.CreateTexture("Resources/texture/", "mario.jpg");
	haikei.CreateTexture("Resources/texture/", "haikei.jpg");

	// サウンド
	sound = std::make_unique<Sound>();
	sound->Init();
	soundData1 = sound->SoundLoadWave("Resources/Sound/Alarm01.wav");
	soundData2 = sound->SoundLoadWave("Resources/Sound/Alarm02.wav");
	soundData3 = sound->SoundLoadWave("Resources/Sound/Alarm03.wav");
}

void GameScence::Init()
{
	objShader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");

	objPipeline = std::make_unique<KGPlin>(objShader, 5);

	if (!ParticleManager::GetInstance()->IsPoolCreated())
	{
		ParticleManager::GetInstance()->CreatePool(hoge.get(), objPipeline.get());
	}

	skydorm = std::make_unique<KObject3d>();
	skydorm->Initialize();
	skydorm->LoadModel(SkyBox.get());
	skydorm->SetPipeline(objPipeline.get());
	skydorm->transform.scale = { 500,500,500 };

	player.Init(hoge.get(), objPipeline.get());

	Player::nowPlayer = &player;

	angle = 0;

	hogeSpeed = 0;

	speedLevel = 1;

	speed = 0;

	hogeLifeTime = 0;

	hogeAngle = 0;

	hogeRot = 0;

	hogeCooltime = 0;

#pragma region スプライト
	sprite = std::make_unique<Sprite>();
	sprite->Init(KDirectXCommon::GetInstance());

	spriteCommon = sprite->SpriteCommonCreate();

	sprite->SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/texture/playerColor.png");
	sprite->SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/texture/bossColor.png");

	for (int i = 0; i < MaxHoge; i++)
	{
		spriteHoge[i] = sprite->SpriteCreate(spriteHoge[i].texNum, spriteCommon);
		spriteHoge[i].size.x = 1;
		spriteHoge[i].size.y = (float)KWinApp::GetWindowSizeH();
		sprite->SpriteTransferVertexBuffer(spriteHoge[i], spriteCommon);
		spriteHoge[i].texNum = 1;
	}
	spriteHoge[0].position = { (float)KWinApp::GetWindowSizeW() / 2, (float)KWinApp::GetWindowSizeH() / 2, 0};
#pragma endregion

#pragma region デバッグテキスト
	debugtext = std::make_unique<DebugText>();

	const int debugTextNumber = 4;
	sprite->SpriteCommonLoadTexture(spriteCommon, debugTextNumber, L"Resources/texture/tex1.png");
	debugtext->Init(sprite.get(), debugTextNumber, spriteCommon);
#pragma endregion
#pragma endregion

	isDebug = true;
	camera = new DebugCamera();
}

void GameScence::Update()
{
	char bufD[255] = "DebugCamera";
	char bufG[255] = "GameCamera";
	ImGui::Text("CameraMode");
	if (ImGui::Button("ChangeCamera"))
	{
		isChange = true;
	}
	if (isDebug)
	{
		ImGui::InputText("string", bufD, IM_ARRAYSIZE(bufD));
	}
	else
	{
		ImGui::InputText("string", bufG, IM_ARRAYSIZE(bufG));
	}

	if (isChange)
	{
		spriteHoge[0].size.x += 20;
		if (spriteHoge[0].size.x >= KWinApp::GetWindowSizeW())
		{
			if (isDebug)
			{
				isDebug = false;
				delete camera;
				camera = new GameCamera();
			}
			else
			{
				isDebug = true;
				delete camera;
				camera = new DebugCamera();
			}
			isChange = false;
		}
	}
	else
	{
		if (spriteHoge[0].size.x > 0)
		{
			spriteHoge[0].size.x -= 20;
		}
	}

	ImGui::Text("Sound");
	if (ImGui::Button("Play1"))
	{
		sound->SoundPlayWave(soundData1);
	}
	if (ImGui::Button("Play2"))
	{
		sound->SoundPlayWave(soundData2);
	}
	if (ImGui::Button("Play3"))
	{
		sound->SoundPlayWave(soundData3);
	}

	float piAngle = PI * 2;

	ImGui::Text("Particle");
	if (ImGui::Button("Splash"))
	{
		while (angle < XMConvertToRadians(360))
		{
			ParticleManager::GetInstance()->TestSplash({ Player::nowPlayer->GetPos().x + (2 * cosf(piAngle + angle)),
				0,
				Player::nowPlayer->GetPos().z + 2 * sinf(piAngle + angle) },
				{ 1,1,1 }, { 1,1,1 }, 3, piAngle + angle, 30);
			angle += XMConvertToRadians(20);
		}
		angle = 0;
		vec.Normalize();
		for (int i = 0; i < 100; i++)
		{
			ParticleManager::GetInstance()->Splash(Player::nowPlayer->GetPos(), {1,1,1}, {1,1,1}, 40, 5, vec);
		}
	}
	if (ImGui::Button("Wave"))
	{
		if (isWave)
		{
			isWave = false;
		}
		else
		{
			isWave = true;
		}
	}
	if (ImGui::Button("Taihun"))
	{
		if (isTaihun)
		{
			isTaihun = false;
		}
		else
		{
			isTaihun = true;
		}
	}

	if (KInput::GetInstance()->IsTrigger(DIK_K))
	{
		if (speedLevel > 1)
		{
			speedLevel--;
		}
	}

	if (KInput::GetInstance()->IsTrigger(DIK_L))
	{
		if (speedLevel < 3)
		{
			speedLevel++;
		}
	}

	if (speedLevel == 1)
	{
		speed = 1;
		hogeLifeTime = 5;
	}
	else if (speedLevel == 2)
	{
		speed = 3;
		hogeLifeTime = 10;
	}
	else if (speedLevel == 3)
	{
		speed = 5;
		hogeLifeTime = 20;
	}
	else
	{
		speed = 0;
		hogeLifeTime = 0;
	}

	if (isWave)
	{
		for (int i = 0; i < 5; i++)
		{
			ParticleManager::GetInstance()->RightWave({ Player::nowPlayer->GetPos().x,
				Player::nowPlayer->GetPos().y + MyMath::GetInstance()->GetRand(-3.0f,3.0f),
				Player::nowPlayer->GetPos().z }, { 1,1,1 }, { 1,1,1 }, 20, speed);
			ParticleManager::GetInstance()->LeftWave({ Player::nowPlayer->GetPos().x,
				Player::nowPlayer->GetPos().y + MyMath::GetInstance()->GetRand(-3.0f,3.0f),
				Player::nowPlayer->GetPos().z }, { 1,1,1 }, { 1,1,1 }, 20, speed);
		}
	}

	if (isTaihun)
	{
		hogeAngle = PI * 2 + hogeRot;
		hogeRot += XMConvertToRadians(10.0f);
		hogeCooltime--;
		if (hogeCooltime <= 0)
		{
			ParticleManager::GetInstance()->Taihun({ 8 * cosf(hogeAngle),8 * sinf(hogeAngle),0 },
				{ 1,1,1 }, { 1,1,1 }, 0.2f, hogeAngle, 40);
			ParticleManager::GetInstance()->Taihun({ 8 * cosf(hogeAngle + XMConvertToRadians(90)),8 * sinf(hogeAngle + XMConvertToRadians(90)),0 },
				{ 1,1,1 }, { 1,1,1 }, 0.2f, hogeAngle + XMConvertToRadians(90), 40);
			ParticleManager::GetInstance()->Taihun({ 8 * cosf(hogeAngle + XMConvertToRadians(180)),8 * sinf(hogeAngle + XMConvertToRadians(180)),0 },
				{ 1,1,1 }, { 1,1,1 }, 0.2f, hogeAngle + XMConvertToRadians(180), 40);
			ParticleManager::GetInstance()->Taihun({ 8 * cosf(hogeAngle + XMConvertToRadians(270)),8 * sinf(hogeAngle + XMConvertToRadians(270)),0 },
				{ 1,1,1 }, { 1,1,1 }, 0.2f, hogeAngle + XMConvertToRadians(270), 40);
			hogeCooltime = 0;
		}
	}
	else
	{
		hogeRot = 0;
		hogeCooltime = 0;
	}

	for (int i = 0; i < MaxHoge; i++)
	{
		sprite->SpriteUpdate(spriteHoge[i], spriteCommon);
	}

	camera->Update();

	// プレイヤー初期化
	player.Update(camera->viewProjection);

	skydorm->Update(camera->viewProjection);

	ParticleManager::GetInstance()->Update(camera->viewProjection);
}

void GameScence::Draw()
{
	player.Draw();

	skydorm->Draw();

	ParticleManager::GetInstance()->Draw();

	// スプライト描画
	sprite->SpriteCommonBeginDraw(spriteCommon);

	if (spriteHoge[0].size.x > 1)
	{
		for (int i = 0; i < MaxHoge; i++)
		{
			sprite->SpriteDraw(spriteHoge[i], spriteCommon);
		}
	}

	debugtext->Print(spriteCommon, "FPS(w)" + std::to_string(KDirectXCommon::GetInstance()->fps), { 10,50 }, 2.0f);
	debugtext->DrawAll(spriteCommon);
}
