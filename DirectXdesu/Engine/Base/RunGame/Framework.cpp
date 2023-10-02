#include "Framework.h"
#include "Sprite.h"
#include "BaseScene.h"
#include "ParticleManager.h"

void Framework::Init()
{
	// FBX������
	fbxLoader = FbxLoader::GetInstance();
	fbxLoader->Init();

	// �E�B���h�E������
	win = KWinApp::GetInstance();
	win->Init();

	// DirectX������
	dx = KDirectXCommon::GetInstance();
	dx->Init();

	// Input������
	input = KInput::GetInstance();
	input->Init();

	sound = Sound::GetInstance();
	sound->Init();

	// �ÓI������
	Sprite::StaticInit();
	PostEffect::StaticInit();

	// �V�[���}�l�[�W���[����
	sceneManager = SceneManager::GetInstance();

	// �e�N�X�`���}�l�[�W���[������
	textureManager = TextureManager::GetInstance();
	textureManager->Init();

	Particles::StaticInitialize();

	// Imgui������
	imguiMane.Init();

	// Sprite�p�C�v���C��
	spriteShader.Init(L"Resources/Shader/PostEffectVS.hlsl", L"Resources/Shader/PostEffectPS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "PostEffect");

	// �|�X�g�G�t�F�N�g�e�X�g
	postEffect = std::make_unique<PostEffect>();
	postEffect->Init();
	postEffect->SetPipeline(spritePipeline.get());

	sceneChange = SceneChange::GetInstance();
	sceneChange->Init();
}

void Framework::Final()
{
	// �V�[���}�l�[�W���[�̉��
	sceneManager->Final();

	// �e�N�X�`���}�l�[�W���[�̉��
	textureManager->Delete();

	// �p�[�e�B�N���}�l�[�W���[���
	ParticleManager::GetInstance()->Delete();
	ObjParticleManager::GetInstance()->Delete();

	// �V�[���t�@�N�g���[���
	delete sceneFactory;

	sceneChange->Delete();

	dx->Destroy();

	fbxLoader->Finalize();
}

void Framework::Update()
{
	// �X�V
	input->Update();

	// Imgui�X�V�͂���
	imguiMane.Begin();

	// �V�[���}�l�[�W���[�̍X�V
	sceneManager->Update();

	// Imgui�X�V�I��
	imguiMane.End();
}

bool Framework::IsEndRwquest()
{
	if (win->ProcessMessage() || input->IsTrigger(DIK_ESCAPE))
	{
		return true;
	}

	return false;
}

void Framework::Run()
{
	Init();

	while (true)
	{
		// �I�����N�G�X�g
		if (IsEndRwquest())
		{
			break;
		}

		// �X�V
		Update();
		// �`��
		Draw();
	}
	// �I��
	Final();
}
