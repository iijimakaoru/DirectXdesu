#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"
#include "TitleCamera.h"

#include "SceneManager.h"

#include <imgui.h>

TitleScene::~TitleScene()
{
	Final();
}

void TitleScene::LoadResources()
{
	// �p�C�v���C��
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "Sprite");

	objShader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	objPipeline.reset(KGPlin::Create(objShader, "Obj"));

	// �^�C�g�����e�N�X�`��
	titleTex = TextureManager::Load("Resources/texture/kariTitle.png");

	// �v�b�V��A�e�N�X�`��
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

	// �^�C�g����
	titleName = std::make_unique<Sprite>();
	titleName->Init();
	titleName->SetPipeline(spritePipeline.get());

	// �v�b�V��A
	pushA = std::make_unique<Sprite>();
	pushA->Init();
	pushA->SetPipeline(spritePipeline.get());

	object3d.reset(KObject3d::Create(model.get(), objPipeline.get()));

	goGame = false;
}

void TitleScene::Update()
{
	object3d->transform.rot.y += 0.5f;

	nowAngle = object3d->GetRot().y;

	object3d->Update(camera->GetViewPro());

	camera->Update();

	// ���̃V�[����
	if (!sceneChange->GetIsEffect())
	{
		if (input->IsTrigger(DIK_SPACE) || input->GetPadButtonDown(XINPUT_GAMEPAD_A))
		{
			sceneChange->Start();
		}
	}

	if (sceneChange->GetIsChange())
	{
		// �V�[���؂�ւ��˗�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::ObjDraw()
{
	object3d->Draw();
}

void TitleScene::SpriteDraw()
{
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	titleName->Draw(titleTex, { width / 2, height * 1 / 3 }, { 1,1 });

	pushA->Draw(pushATex, { width / 2, height * 2 / 3 }, { 1,1 });
}

void TitleScene::Final()
{
	
}
