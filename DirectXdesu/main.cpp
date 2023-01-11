#include <stdio.h>
#include <d3dcompiler.h>
#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#include "KDepth.h"
#include "KTexture.h"
#include "KObject3d.h"
#include "ViewProjection.h"
#include "KModel.h"
#include "KShader.h"
#ifdef _DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")
#include "Sound.h"
#include <d3dx12.h>
#include "ConstBuffer.h"
#include "PipelineSet.h"
#include "Vector4.h"
#include "Sprite.h"
#include "DebugText.h"

#include <array>

#include"Player.h"
#include"Stage.h"
#include"Boss.h"
#include "BossBulletManager.h"
#include "DebugCamera.h"
#include "Camera.h"
#include "MyMath.h"
#include "ParticleManager.h"

PipelineSet Create3DObjectGpipeline()
{
	HRESULT result;
#pragma region �V�F�[�_�[�ǂݍ��݂ƃR���p�C��
	std::unique_ptr<KShader> shader;
	shader = std::make_unique<KShader>();
	shader->ObjVSLoadCompile();
	shader->ObjPSLoadCompile();
#pragma endregion
#pragma region ���_���C�A�E�g�z��̐錾�Ɛݒ�
	static D3D12_INPUT_ELEMENT_DESC inputLayout[3] = {
	{// xyz���W
		"POSITION",										// �Z�}���e�B�b�N��
		0,												// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X
		DXGI_FORMAT_R32G32B32_FLOAT,					// �v�f���ƃr�b�g����\��
		0,												// ���̓X���b�g�C���f�b�N�X
		D3D12_APPEND_ALIGNED_ELEMENT,					// �f�[�^�̃I�t�Z�b�g
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// ���̓f�[�^���
		0												// ��x�ɕ`�悷��C���X�^���X��
	},
	{// �@���x�N�g��
		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	{// uv���W
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	};
#pragma endregion
#pragma region �p�C�v���C���X�e�[�g�ݒ�ϐ��̐錾�Ɗe�퍀�ڂ̐ݒ�
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	// �V�F�[�_�[�̐ݒ�
	gpipeline.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = shader->psBlob->GetBufferSize();
	// �T���v���}�X�N�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ���X�^���C�U�̐ݒ�
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;

	// �u�����h�X�e�[�g
	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpipeline.SampleDesc.Count = 1;

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParam[3] = {};
	// �萔�o�b�t�@0��
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[0].Descriptor.ShaderRegister = 0;
	rootParam[0].Descriptor.RegisterSpace = 0;
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// �e�N�X�`�����W�X�^0��
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// �萔�o�b�t�@1��
	rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[2].Descriptor.ShaderRegister = 1;
	rootParam[2].Descriptor.RegisterSpace = 0;
	rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// �e�N�X�`���T���u���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
#pragma endregion
	// �p�C�v���C���ƃ��[�g�V�O�l�`���̃Z�b�g
	PipelineSet pipelineSet;
#pragma region ���[�g�V�O�l�`���̐���
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = _countof(rootParam);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader->errorBlob);
	assert(SUCCEEDED(result));
	result = KDirectXCommon::GetInstance()->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootSignature));
	assert(SUCCEEDED(result));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = pipelineSet.rootSignature.Get();
#pragma endregion
#pragma region �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
	// �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState.DepthEnable = true; // �[�x�e�X�g
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // �������݋���
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // ��������΍��i
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g

	result = KDirectXCommon::GetInstance()->GetDev()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

	return pipelineSet;
}

bool BoxCollision(WorldTransfom& transformA, WorldTransfom& transformB);

void AllCollision();

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#pragma region ��Տ�����
#pragma region �E�B���h�E
	KWinApp::Init();
#pragma endregion
#pragma region DirectX������
	KDirectXCommon::Init();
	// �L�[�{�[�h����
	KInput::Init();
#pragma endregion
#pragma endregion

#pragma region �V�[���̏�����
	// ����
	float speed = 1.0f;
#pragma region ���f��
	KModel triangle = Triangle();
	triangle.CreateModel();
	KModel cube = Cube();
	cube.CreateModel();
	KModel line = Line();
	line.CreateModel();
	KModel objTriangle = MtlObj("triangle_mat");
	objTriangle.CreateModel();
	KModel piramid = MtlObj("pramid");
	piramid.CreateModel();
	KModel tekitou = MtlObj("tekitou");
	tekitou.CreateModel();
	KModel boxSky = MtlObj("boxSky");
	boxSky.CreateModel();
#pragma endregion
#pragma region �e�N�X�`��������
	KTexture mario;
	mario.CreateTexture("Resources/texture/", "mario.jpg");
	KTexture kitanai;
	kitanai.CreateTexture("Resources/texture/", "kitanai.jpg");
	KTexture haikei;
	haikei.CreateTexture("Resources/texture/", "haikei.jpg");
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�
	// 3D�I�u�W�F�N�g�p�p�C�v���C������
	PipelineSet object3dPipelineSet = Create3DObjectGpipeline();

	// �v���C���[
	Player player(&cube);

	Player::nowPlayer = &player;

	// �{�X
	Boss boss(&cube);

	Boss::nowBoss = &boss;

	if (!BossBulletManager::GetInstance()->IsPoolCreated())
	{
		BossBulletManager::GetInstance()->CreatePool(&cube);
	}

	if (!ParticleManager::GetInstance()->IsPoolCreated())
	{
		ParticleManager::GetInstance()->CreatePool(&cube);
	}

	// �X�e�[�W
	Stage stage;
	stage.Init(&cube);

	// �V��
	KObject3d boxSkydorm;
	boxSkydorm.Initialize();
	boxSkydorm.LoadModel(&boxSky);

	boxSkydorm.transform.scale = { 320,320,320 };

#pragma region �r���[
	// �r���[�v���W�F�N�V����
	ViewProjection viewProjection;
	viewProjection.Initialize();
	viewProjection.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();

	//// �J����
	//Camera camera;

	//// �f�o�b�O�J����
	//DebugCamera debugCamera({ 0,0,-10 });

	//Camera::nowCamera = &debugCamera;
#pragma endregion

#pragma endregion
	Vector3 center = { 0,0,1 };

	float rSpeed = -0.02f;
	float gSpeed = 0.02f;
	float bSpeed = -0.02f;
	float aSpeed = -0.02f;

	std::unique_ptr<Sound> sound;
	sound = std::make_unique<Sound>();
	sound->Init();

	SoundData soundData1 = sound->SoundLoadWave("Resources/Sound/fanfare.wav");
#pragma region �X�v���C�g
	Sprite sprite;
	sprite.Init(KDirectXCommon::GetInstance());

	SpriteCommon spriteCommon;
	spriteCommon = sprite.SpriteCommonCreate();

	sprite.SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/texture/playerColor.png");
	sprite.SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/texture/bossColor.png");

	// �v���C���[HP��UI
	static const int pMaxHP = 3;
	std::array<SpriteInfo,pMaxHP> playersHP;
	for (int i = 0; i < pMaxHP; i++)
	{
		playersHP[i] = sprite.SpriteCreate(playersHP[i].texNum, spriteCommon);
		playersHP[i].size.x = 100.0f;
		playersHP[i].size.y = 100.0f;
		sprite.SpriteTransferVertexBuffer(playersHP[i], spriteCommon);
		playersHP[i].texNum = 0;
	}
	playersHP[0].position = { 60, 650, 0 };
	playersHP[1].position = { 180, 650, 0 };
	playersHP[2].position = { 300, 650, 0 };

	// �{�XHP��UI
	static const int bMaxHP = 20;
	std::array<SpriteInfo, bMaxHP> bosssHP;
	for (int i = 0; i < bMaxHP; i++)
	{
		bosssHP[i] = sprite.SpriteCreate(bosssHP[i].texNum, spriteCommon);
		bosssHP[i].size.x = 25.0f;
		bosssHP[i].size.y = 50.0f;
		sprite.SpriteTransferVertexBuffer(bosssHP[i], spriteCommon);
		bosssHP[i].texNum = 1;
		bosssHP[0].position = { 400,50,0 };
		if (i > 0)
		{
			bosssHP[i].position = bosssHP[i - 1].position;
			bosssHP[i].position.x += 25;
		}
	}
	
	
#pragma endregion

#pragma region �f�o�b�O�e�L�X�g
	DebugText* debugtext = new DebugText();

	const int debugTextNumber = 2;
	sprite.SpriteCommonLoadTexture(spriteCommon, debugTextNumber, L"Resources/texture/tex1.png");
	debugtext->Init(&sprite, debugTextNumber, spriteCommon);
#pragma endregion
#pragma endregion

	enum class Scene
	{
		Title,
		Play,
		Clear,
		Over,
	};

	Scene gameScene = Scene::Title;

	bool isTexture = false;

	// �E�B���h�E�\��
	// �Q�[�����[�v
	while (true)
	{
#pragma region �E�B���h�E���b�Z�[�W
		if (KWinApp::GetInstance()->ProcessMessage() || KInput::GetInstance()->IsPush(DIK_ESCAPE))
		{
			break;
		}
#pragma endregion

		// �X�V
#pragma region ��Ղ̍X�V
		// input�X�V
		KInput::Update();
#pragma endregion

#pragma region �V�[���̍X�V
		for (int i = 0; i < pMaxHP; i++)
		{
			sprite.SpriteUpdate(playersHP[i], spriteCommon);
		}

		for (int i = 0; i < bMaxHP; i++)
		{
			sprite.SpriteUpdate(bosssHP[i], spriteCommon);
		}

		if (gameScene == Scene::Title)
		{
			// �v���C���[������
			player.Init();

			// �{�X������
			boss.Init();

			// �o�Ă�e��S�ď���
			BossBulletManager::GetInstance()->AllDelete();

			// �o�Ă�p�[�e�B�N����S�ď���
			ParticleManager::GetInstance()->AllDelete();

			if (KInput::GetInstance()->IsTriger(DIK_SPACE))
			{
				gameScene = Scene::Play;
				boss.startFlag = true;
			}
		}
		if (gameScene == Scene::Play)
		{
			player.Update(viewProjection);

			boss.Update(Player::nowPlayer->view);

			AllCollision();

			if (KInput::GetInstance()->IsTriger(DIK_Z))
			{
				for (int i = 0; i < 50; i++)
				{
					ParticleManager::GetInstance()->Explosion(Player::nowPlayer->object.transform.pos, { 4,4,4 }, { 1,1,1 }, 60, 10);
				}
			}

			if (Player::nowPlayer->hp <= 0)
			{
				gameScene = Scene::Over;
			}

			if (Boss::nowBoss->hp <= 0)
			{
				gameScene = Scene::Over;
			}

			BossBulletManager::GetInstance()->Update(Player::nowPlayer->view);

			ParticleManager::GetInstance()->Update(Player::nowPlayer->view);
		}
		if (gameScene == Scene::Clear)
		{
			if (KInput::GetInstance()->IsTriger(DIK_SPACE))
			{
				gameScene = Scene::Title;
			}
		}
		if (gameScene == Scene::Over)
		{
			if (KInput::GetInstance()->IsTriger(DIK_SPACE))
			{
				gameScene = Scene::Title;
			}
		}

		stage.Update(Player::nowPlayer->view);

		// �r���[�̃A�b�v�f�[�g
		viewProjection.Update();

		boxSkydorm.Update(Player::nowPlayer->view);
#pragma endregion

#pragma region �`��
		// �`��J�n
		KDirectXCommon::PreDraw();
#pragma region �p�C�v���C���X�e�[�g�ݒ�
		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		KDirectXCommon::GetInstance()->GetCmdlist()->SetPipelineState(object3dPipelineSet.pipelineState.Get());
		KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootSignature(object3dPipelineSet.rootSignature.Get());
#pragma endregion
#pragma region �v���~�e�B�u�`��
		// �v���~�e�B�u�`��̐ݒ�R�}���h
		KDirectXCommon::GetInstance()->GetCmdlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region �`��R�}���h
		// �`��R�}���h
		if (gameScene == Scene::Title)
		{

		}

		if (gameScene == Scene::Play)
		{
			player.Draw();

			boss.Draw();

			BossBulletManager::GetInstance()->Draw();

			ParticleManager::GetInstance()->Draw();

			stage.Draw(&mario);
		}

		if (gameScene == Scene::Clear)
		{

		}

		if (gameScene == Scene::Over)
		{

		}

		boxSkydorm.Draw();

		// �X�v���C�g�`��
		sprite.SpriteCommonBeginDraw(spriteCommon);
		if (gameScene == Scene::Play)
		{
#pragma region �v���C���[��HP
			if (Player::nowPlayer->hp >= 1)
			{
				sprite.SpriteDraw(playersHP[0], spriteCommon);
			}

			if (Player::nowPlayer->hp >= 2)
			{
				sprite.SpriteDraw(playersHP[1], spriteCommon);
			}

			if (Player::nowPlayer->hp >= 3)
			{
				sprite.SpriteDraw(playersHP[2], spriteCommon);
			}
#pragma endregion

#pragma region �{�X��HP
			if (Boss::nowBoss->hp >= 1)
			{
				sprite.SpriteDraw(bosssHP[0], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 2)
			{
				sprite.SpriteDraw(bosssHP[1], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 3)
			{
				sprite.SpriteDraw(bosssHP[2], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 4)
			{
				sprite.SpriteDraw(bosssHP[3], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 5)
			{
				sprite.SpriteDraw(bosssHP[4], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 6)
			{
				sprite.SpriteDraw(bosssHP[5], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 7)
			{
				sprite.SpriteDraw(bosssHP[6], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 8)
			{
				sprite.SpriteDraw(bosssHP[7], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 9)
			{
				sprite.SpriteDraw(bosssHP[8], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 10)
			{
				sprite.SpriteDraw(bosssHP[9], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 11)
			{
				sprite.SpriteDraw(bosssHP[10], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 12)
			{
				sprite.SpriteDraw(bosssHP[11], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 13)
			{
				sprite.SpriteDraw(bosssHP[12], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 14)
			{
				sprite.SpriteDraw(bosssHP[13], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 15)
			{
				sprite.SpriteDraw(bosssHP[14], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 16)
			{
				sprite.SpriteDraw(bosssHP[15], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 17)
			{
				sprite.SpriteDraw(bosssHP[16], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 18)
			{
				sprite.SpriteDraw(bosssHP[17], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 19)
			{
				sprite.SpriteDraw(bosssHP[18], spriteCommon);
			}
			if (Boss::nowBoss->hp >= 20)
			{
				sprite.SpriteDraw(bosssHP[19], spriteCommon);
			}
#pragma endregion
		}

		debugtext->Print(spriteCommon, "FPS(w)" + std::to_string(KDirectXCommon::GetInstance()->fps), { 10,50 }, 2.0f);
		debugtext->DrawAll(spriteCommon);

		// �`��R�}���h�����܂�
#pragma endregion
		// �`��I��
		KDirectXCommon::PostDraw();
	}

#pragma region ��Ղ̏I��
	sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);
#pragma endregion

	return 0;
}

bool BoxCollision(WorldTransfom& transformA, WorldTransfom& transformB)
{
	if (transformA.pos.x - transformA.scale.x <= transformB.pos.x + transformB.scale.x && // posA����posB�E�̔���
		transformA.pos.x + transformA.scale.x >= transformB.pos.x - transformB.scale.x && // posA�E��posB���̔���
		transformA.pos.y - transformA.scale.y <= transformB.pos.y + transformB.scale.y && // posA����posB��̔���
		transformA.pos.y + transformA.scale.y >= transformB.pos.y - transformB.scale.y && // posA���posB���̔���
		transformA.pos.z - transformA.scale.z <= transformB.pos.z + transformB.scale.z && // posA�O��posB���̔���
		transformA.pos.z + transformA.scale.z >= transformB.pos.z - transformB.scale.z)   // posA����posB�O�̔���
	{
		return true;
	}

	return false;
}

void AllCollision()
{
	WorldTransfom transform;

	for (std::unique_ptr<BossBullet>& bullet : BossBulletManager::GetInstance()->bossBullets)
	{
		transform = bullet->object.transform;
		if (BoxCollision(Player::nowPlayer->object.transform, transform) && !bullet->IsDead())
		{
			Player::nowPlayer->damageTimer = 60;
			Player::nowPlayer->isDamage = true;
			bullet->isDead = true;
		}
	}

	if (BoxCollision(Player::nowPlayer->object.transform, Boss::nowBoss->object.transform) && Player::nowPlayer->isDash)
	{
		Player::nowPlayer->hitTimer = 20;
		Player::nowPlayer->isHit = true;
		Boss::nowBoss->Damage();
	}
}