#include "GameScence.h"
#include "KInput.h"

#include "ParticleManager.h"
#include "MyMath.h"

#include "DebugCamera.h"
#include "GameCamera.h"

#include "Collision.h"
#include <sstream>
#include <iomanip>

#include "FbxLoader.h"

GameScence::~GameScence()
{
	delete camera;
	sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);
	delete partMan;

	delete object1;
	delete fbxModel1;
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
	// Obj
	objShader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	objPipeline = std::make_unique<KGPlin>();
	objPipeline->CreatePipeline(objShader, 5);

	// Fbx
	fbxShader.Init(L"Resources/Shader/FBXVS.hlsl", L"Resources/Shader/FBXPS.hlsl");
	fbxPipeline = std::make_unique<KGPlin>();
	fbxPipeline->CreatePipelineAll(fbxShader, false, false, false, true);

#pragma region Fbxテスト
	fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

	object1 = new FbxObject3D;
	object1->Init();
	object1->SetPipline(fbxPipeline.get());
	object1->SetModel(fbxModel1);
#pragma endregion


	if (!ParticleManager::GetInstance()->IsPoolCreated())
	{
		ParticleManager::GetInstance()->CreatePool(hoge.get(), objPipeline.get());
	}

	skydorm = std::make_unique<KObject3d>();
	skydorm->Initialize();
	skydorm->LoadModel(SkyBox.get());
	skydorm->SetPipeline(objPipeline.get());
	skydorm->transform.scale = { 500,500,500 };

	testTriangle = std::make_unique<KObject3d>();
	testTriangle->Initialize();
	testTriangle->LoadModel(triangle.get());
	testTriangle->SetPipeline(objPipeline.get());
	testTriangle->transform.scale = { 100,100,1 };
	testTriangle->transform.pos.y = -10;
	testTriangle->transform.rot.x = XMConvertToRadians(90);

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
		sprite->SpriteTransferVertexBuffer(spriteHoge[i], spriteCommon);
		spriteHoge[i].texNum = 1;
	}
	spriteHoge[0].size.x = 1;
	spriteHoge[0].size.y = (float)KWinApp::GetWindowSizeH();
	spriteHoge[0].position = { (float)KWinApp::GetWindowSizeW() / 2, (float)KWinApp::GetWindowSizeH() / 2, 0 };
	spriteHoge[1].position = { 5,5,0 };
	spriteHoge[1].size.x = 5;
	spriteHoge[1].size.y = 5;
	spriteHoge[1].texNum = 0;
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

	// コライダーモード
	colMode = CollisionMode::Sphere_Plane;

	stage = std::make_unique<KObject3d>();
	stage->transform.scale = { 80,0.1f,80 };
	stage->transform.pos.y = -10;
	stage->LoadModel(cube.get());
	stage->SetPipeline(objPipeline.get());

	// 球の初期値を設定
	sphere.center = XMVectorSet(player.GetPos().x, player.GetPos().y, player.GetPos().z, 1); // 中心点座標
	sphere.radius = 100.0f; // 半径

	// 平面の初期値を設定
	plane.normal = XMVectorSet(0, 1, 0, 0); // 法線ベクトル
	plane.distance = 0.0f; // 原点(0,0,0)からの距離

	// 三角形の初期値設定
	colTriangle.p0 = XMVectorSet(-50.0f, 0, -50.0f, 1);
	colTriangle.p1 = XMVectorSet(0.0f, 0, +50.0f, 1);
	colTriangle.p2 = XMVectorSet(+50.0f, 0, -50.0f, 1);
	colTriangle.normal = XMVectorSet(0, 1, 0, 0);

	// レイの初期値設定
	ray.start = XMVectorSet(0, 1, 0, 1); // 原点やや上
	ray.dir = XMVectorSet(0, -1, 0, 0); // 下向き

	// 3Dオブジェクト生成
	partMan = BillParticleManager::Create(L"Resources/texture/playerColor.png");
	for (int i = 0; i < 1; i++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		partMan->Add(100, pos, vel, acc, 1.0f, 0.0f);
	}

	partMan->Update(camera->viewProjection);

	FbxLoader::GetInstance()->LoadModelFromFile("test");
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
		ImGui::InputText("mode", bufD, IM_ARRAYSIZE(bufD));
	}
	else
	{
		ImGui::InputText("mode", bufG, IM_ARRAYSIZE(bufG));
	}

	if (isChange)
	{
		spriteHoge[0].size.x += 2;
		if (spriteHoge[0].size.x >= KWinApp::GetWindowSizeW() / 10)
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
			spriteHoge[0].size.x -= 2;
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
			ParticleManager::GetInstance()->Splash(Player::nowPlayer->GetPos(), { 1,1,1 }, { 1,1,1 }, 40, 5, vec);
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

	if (ImGui::Button("Sphere_Plane"))
	{
		colMode = CollisionMode::Sphere_Plane;
	}
	if (ImGui::Button("Sphere_Triangle"))
	{
		colMode = CollisionMode::Sphere_Triangle;
	}
	if (ImGui::Button("Ray_Plane"))
	{
		ray.dir.m128_f32[1] = 1;
		colMode = CollisionMode::Ray_Plane;
	}
	if (ImGui::Button("Ray_Triangle"))
	{
		colMode = CollisionMode::Ray_Triangle;
	}

	stage->Update(camera->viewProjection);
	plane.normal.m128_f32[0] = stage->transform.pos.x;
	plane.normal.m128_f32[1] = stage->transform.pos.y;
	plane.normal.m128_f32[2] = stage->transform.pos.z;

	testTriangle->Update(camera->viewProjection);

	for (int i = 0; i < MaxHoge; i++)
	{
		sprite->SpriteUpdate(spriteHoge[i], spriteCommon);
	}

	camera->Update();

	Vector3 playerPos = player.GetPos();
	ImGui::Text("Player");
	ImGui::Text("pos: (%.2f,%.2f,%.2f)", playerPos.x, playerPos.y, playerPos.z);
	ImGui::SliderFloat("playerPosX", &playerPos.x, -60.0f, 60.0f);

	player.SetPos({ playerPos.x,playerPos.y,playerPos.z });

	// プレイヤー初期化
	player.Update(camera->viewProjection);
	// 球判定
	sphere.center.m128_f32[0] = player.GetPos().x;
	sphere.center.m128_f32[1] = player.GetPos().y + 10;
	sphere.center.m128_f32[2] = player.GetPos().z;
	// レイ判定
	ray.start.m128_f32[0] = player.GetPos().x;
	ray.start.m128_f32[1] = player.GetPos().y;
	ray.start.m128_f32[2] = player.GetPos().z;

	if (colMode == CollisionMode::Sphere_Plane)
	{
		sphere.radius = 100;
		isHit = Collision::CheckSphere2Plane(sphere, plane);
	}
	else if (colMode == CollisionMode::Sphere_Triangle)
	{
		sphere.radius = 10;
		isHit = Collision::CheckSphere2Triangle(sphere, colTriangle);
	}
	else if (colMode == CollisionMode::Ray_Plane)
	{
		XMVECTOR inter;
		float distance;
		isHit = Collision::CheckRay2Plane(ray, plane, &distance, &inter);
	}
	else if (colMode == CollisionMode::Ray_Triangle)
	{
		XMVECTOR inter;
		float distance;
		isHit = Collision::CheckRay2Triangle(ray, colTriangle, &distance, &inter);
	}

	skydorm->Update(camera->viewProjection);

	ParticleManager::GetInstance()->Update(camera->viewProjection);

	for (int i = 0; i < 10; i++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		partMan->Add(100, pos, vel, acc, 1.0f, 0.0f);
	}

	partMan->Update(camera->viewProjection);

	XMFLOAT3 fbxPos = object1->GetPosition();
	ImGui::Text("Fbx");
	ImGui::Text("pos: (%.2f,%.2f,%.2f)", fbxPos.x, fbxPos.y, fbxPos.z);
	ImGui::SliderFloat("FbxPosX", &fbxPos.x, -60.0f, 60.0f);

	object1->SetPosition({ fbxPos.x,fbxPos.y,fbxPos.z });

	// Fbx
	object1->Update(camera->viewProjection);
}

void GameScence::Draw()
{
	player.Draw();

	if (isHit)
	{
		if (colMode == CollisionMode::Sphere_Plane)
		{
			stage->Draw(&haikei);
		}
		else if (colMode == CollisionMode::Sphere_Triangle)
		{
			testTriangle->Draw(&haikei);
		}
		else if (colMode == CollisionMode::Ray_Plane)
		{
			stage->Draw(&haikei);
		}
		else if (colMode == CollisionMode::Ray_Triangle)
		{
			testTriangle->Draw(&haikei);
		}
	}
	else
	{
		if (colMode == CollisionMode::Sphere_Plane)
		{
			stage->Draw(&mario);
		}
		else if (colMode == CollisionMode::Sphere_Triangle)
		{
			testTriangle->Draw(&mario);
		}
		else if (colMode == CollisionMode::Ray_Plane)
		{
			stage->Draw(&mario);
		}
		else if (colMode == CollisionMode::Ray_Triangle)
		{
			testTriangle->Draw(&mario);
		}
	}

	object1->Draw();

	skydorm->Draw();

	ParticleManager::GetInstance()->Draw();

	BillParticleManager::PreDraw(KDirectXCommon::GetInstance()->GetCmdlist());

	partMan->Draw();

	BillParticleManager::PostDraw();

	// スプライト描画
	sprite->SpriteCommonBeginDraw(spriteCommon);

	if (spriteHoge[0].size.x > 1)
	{
		sprite->SpriteDraw(spriteHoge[0], spriteCommon);
	}

	sprite->SpriteDraw(spriteHoge[1], spriteCommon);

	debugtext->Print(spriteCommon, "FPS(w)" + std::to_string(KDirectXCommon::GetInstance()->fps), { 10,50 }, 0.5f);
	debugtext->DrawAll(spriteCommon);
}
