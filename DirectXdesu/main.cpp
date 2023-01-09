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

#include"Player.h"
#include"Stage.h"
#include"Boss.h"
#include "BossBulletManager.h"
#include "DebugCamera.h"
#include "Camera.h"

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
	Player player;
	player.Init(&cube);

	Player::nowPlayer = &player;

	// �{�X
	Boss boss;
	boss.Init(&cube);

	Boss::nowBoss = &boss;

	if (!BossBulletManager::GetInstance()->IsPoolCreated())
	{
		BossBulletManager::GetInstance()->CreatePool(&cube);
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

	sprite.SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/texture/haikei.jpg");
	sprite.SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/texture/mario.jpg");

	SpriteInfo sprites[2];
	for (int i = 0; i < _countof(sprites); i++)
	{
		sprites[i] = sprite.SpriteCreate(sprites[i].texNum, spriteCommon);
		sprites[i].size.x = 100.0f;
		sprites[i].size.y = 100.0f;
		sprite.SpriteTransferVertexBuffer(sprites[i], spriteCommon);
	}
	sprites[0].texNum = 0;
	sprites[1].texNum = 1;
	sprites[1].rotation = 0;
	sprites[1].position.y = 720 / 2, 0;
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
		// �X�v���C�g�̃A�b�v�f�[�g
		for (int i = 0; i < _countof(sprites); i++)
		{
			sprite.SpriteUpdate(sprites[i], spriteCommon);
		}

		//�J��������
		if (KInput::GetInstance()->IsPush(DIK_D))
		{
			viewProjection.angleX += XMConvertToRadians(1.0f);
		}
		else if (KInput::GetInstance()->IsPush(DIK_A))
		{
			viewProjection.angleX -= XMConvertToRadians(1.0f);
		}
		if (KInput::GetInstance()->IsPush(DIK_W))
		{
			viewProjection.angleY -= XMConvertToRadians(1.0f);
		}
		else if (KInput::GetInstance()->IsPush(DIK_S))
		{
			viewProjection.angleY += XMConvertToRadians(1.0f);
		}
		// angle���W�A��y����]
		viewProjection.eye.x = viewProjection.lenZ * sinf(viewProjection.angleX) + Player::nowPlayer->object.transform.pos.x;
		viewProjection.eye.y = viewProjection.lenZ * sinf(viewProjection.angleY) + Player::nowPlayer->object.transform.pos.y;
		viewProjection.eye.z = viewProjection.lenZ * cosf(viewProjection.angleX) * cosf(viewProjection.angleY) + Player::nowPlayer->object.transform.pos.z;

		viewProjection.target.x = Player::nowPlayer->object.transform.pos.x;
		viewProjection.target.y = Player::nowPlayer->object.transform.pos.y + 1;
		viewProjection.target.z = Player::nowPlayer->object.transform.pos.z;

		/*viewProjection.target.x = viewProjection.eye.x - 100 * cosf(PI / 180 * viewProjection.angleX) * cosf(PI / 180 * viewProjection.angleY);
		viewProjection.target.y = viewProjection.eye.y + 100 * sinf(PI / 180 * viewProjection.angleY);
		viewProjection.target.z = viewProjection.eye.z + 100 * sinf(PI / 180 * viewProjection.angleX) * cosf(PI / 180 * viewProjection.angleY);*/

		if (gameScene == Scene::Title)
		{
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

			BossBulletManager::GetInstance()->Update(Player::nowPlayer->view);
		}
		if (gameScene == Scene::Clear)
		{

		}
		if (gameScene == Scene::Over)
		{

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
		player.Draw();

		boss.Draw();

		BossBulletManager::GetInstance()->Draw();

		stage.Draw(&mario);

		boxSkydorm.Draw();

		// �X�v���C�g�`��
		sprite.SpriteCommonBeginDraw(spriteCommon);

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