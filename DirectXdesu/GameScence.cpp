#include "GameScence.h"
#include "KInput.h"

#include "ParticleManager.h"
#include "MyMath.h"

#include "DebugCamera.h"
#include "GameCamera.h"

GameScence::~GameScence()
{
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
}

void GameScence::Init()
{
	objShader.Init(L"ObjVS.hlsl", L"ObjPS.hlsl");

	objPipeline = std::make_unique<KGPlin>(objShader);

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

	angle = 0;

	hogeSpeed = 0;

	speedLevel = 1;

	speed = 0;

	hogeLifeTime = 0;

	hogeAngle = 0;

	hogeRot = 0;

	hogeCooltime = 0;

#pragma region スプライト
	sprite = std::make_unique<hogeSprite>();
	sprite->Init(KDirectXCommon::GetInstance());

	spriteCommon = sprite->SpriteCommonCreate();
#pragma endregion

#pragma region デバッグテキスト
	debugtext = std::make_unique<DebugText>();

	const int debugTextNumber = 4;
	sprite->SpriteCommonLoadTexture(spriteCommon, debugTextNumber, L"Resources/texture/tex1.png");
	debugtext->Init(sprite.get(), debugTextNumber, spriteCommon);
#pragma endregion
#pragma endregion

	camera = new GameCamera();
}

void GameScence::Update()
{
	char buf[255] = "unti";
	ImGui::Text("Hello, world %d", 123);
	if (ImGui::Button("Save"))
	{

	}
	ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
	ImGui::SliderFloat("float", &hogehoge, 5.0f, 20.0f);

	if (KInput::GetInstance()->IsTrigger(DIK_0))
	{
		sound->SoundPlayWave(soundData1);
	}

	float piAngle = PI * 2;

	if (KInput::GetInstance()->IsTrigger(DIK_SPACE))
	{
		while (angle < XMConvertToRadians(360))
		{
			ParticleManager::GetInstance()->TestSplash({ 2 * cosf(piAngle + angle),0,2 * sinf(piAngle + angle) },
				{ 1,1,1 }, { 1,1,1 }, 5 - 2, piAngle + angle, 30);
			angle += XMConvertToRadians(20);
		}
		angle = 0;
		vec.Normalize();
		for (int i = 0; i < 100; i++)
		{
			ParticleManager::GetInstance()->Splash({ 0,0,0 }, { 1,1,1 }, { 1,1,1 }, 40, 5, vec);
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

	if (KInput::GetInstance()->IsPress(DIK_1))
	{
		for (int i = 0; i < 5; i++)
		{
			ParticleManager::GetInstance()->RightWave({ 0,MyMath::GetInstance()->GetRand(-3.0f,3.0f),0 }, { 1,1,1 }, { 1,1,1 }, 20, speed);
			ParticleManager::GetInstance()->LeftWave({ 0,MyMath::GetInstance()->GetRand(-3.0f,3.0f),0 }, { 1,1,1 }, { 1,1,1 }, 20, speed);
		}
	}

	if (KInput::GetInstance()->IsPress(DIK_2))
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

	debugtext->Print(spriteCommon, "FPS(w)" + std::to_string(KDirectXCommon::GetInstance()->fps), { 10,50 }, 2.0f);
	debugtext->DrawAll(spriteCommon);
}
