#include "TitleScene.h"

#include "SceneManager.h"

#include "ModelManager.h"
#include "PipelineManager.h"

TitleScene::~TitleScene() { Final(); }

void TitleScene::LoadResources() {
	// 天球モデル
	skyDomeModel = ModelManager::GetInstance()->GetModels("S_SkyDorm");
	logoModel = ModelManager::GetInstance()->GetModels("titleLogo");

	texBG = TextureManager::Load("Resources/texture/titleBg.png");
	texPressA = TextureManager::Load("Resources/texture/pressA.png");
}

void TitleScene::Init() {
	BaseScene::Init();

	// インスタンス
	input = KInput::GetInstance();

	light_.reset(Light::Create());
	light_->SetLightRGB({1.0f, 1.0f, 1.0f});
	KObject3d::SetLight(light_.get());

	// カメラ読み込み
	camera = std::make_unique<GameCamera>();
	camera->Init();

	sceneManager = SceneManager::GetInstance();

	skyDome.reset(
	    KObject3d::Create(skyDomeModel, PipelineManager::GetInstance()->GetPipeline("Obj")));
	skyDome->GetTransform().SetScale({ skydomeSize, skydomeSize, skydomeSize });
	skyDome->GetTransform().SetPos({ 0.0f, 100.0f, 500.0f });

	logo.reset(KObject3d::Create(logoModel, PipelineManager::GetInstance()->GetPipeline("Obj")));
	logo->GetTransform().SetScale({ 100.0f, 100.0f, 100.0f });
	logo->GetTransform().SetPos({ 0.0f, 60.0f, 100.0f });
	logo->GetTransform().SetRot({ 0.0f, 180.0f, 0.0f});

	backGround.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));
	pressA.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	audioManager = AudioManager::GetInstance();
}

void TitleScene::Update() {
	light_->Update();

	skyDome->Update(camera->GetViewPro(), camera->GetWorldPos());
	logo->Update(camera->GetViewPro(), camera->GetWorldPos());

	logo->GetTransform().SetRot({ 0.0f, 180.0f + RotationLogoY(rotationSpeed), 0.0f});

	camera->Update();
	
	GoNextScene();
}

void TitleScene::ObjDraw() {
	skyDome->Draw();
	backGround->Draw(texBG, { 640.0f,360.0f });
	logo->Draw();
	pressA->Draw(texPressA, { 640.0f,600.0f },{0.8f,0.8f});
}

void TitleScene::SpriteDraw()
{
	
}

void TitleScene::Final() {

}

void TitleScene::GoNextScene() {
	if (input->GetPadButtonDown(A)) {
		sceneManager->ChangeScene("GAME");
	}
	else if (input->IsTrigger(DIK_SPACE)) {
		sceneManager->ChangeScene("GAME");
	}

	if (input->IsPress(DIK_LSHIFT) && input->IsPress(DIK_RSHIFT))
	{
		sceneManager->ChangeScene("SETTING");
	}
}

float TitleScene::RotationLogoY(const float& speed)
{
	if (flag == true)
	{
		result += speed;
	}

	if (result >= 360.0f) 
	{
		flag = false;
		result = 0.0f;
	}

	if (flag == false)
	{
		timer++;
		//3秒経過で再スタート
		if (timer >= 180.0f)
		{
			flag = true;
			timer = 0.0f;
		}
	}

	return result;
}
