#include <stdio.h>
#include <d3dcompiler.h>
#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#include "KDepth.h"
#include "KTexture.h"
#include "KWorldTransform.h"
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

PipelineSet Create3DObjectGpipeline(ID3D12Device* dev)
{
	HRESULT result;
#pragma region �V�F�[�_�[�ǂݍ��݂ƃR���p�C��
	std::unique_ptr<KShader> shader;
	shader = std::make_unique<KShader>();
	shader->BasicPSLoadCompile();
	shader->BasicVSLoadCompile();
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
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
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

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

	return pipelineSet;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	HRESULT result;
#pragma region ��Տ�����
#pragma region �E�B���h�E
	KWinApp* win = nullptr;
	win = new KWinApp();
#pragma endregion
#pragma region DirectX������
	// KDirectCommon
	KDirectXCommon* dxCommon = nullptr;
	dxCommon = new KDirectXCommon();
	dxCommon->Init(win);
	// �L�[�{�[�h����
	KInput* input = nullptr;
	input = new KInput();
	input->Init(win);
#pragma endregion
#pragma endregion

#pragma region �V�[���̏�����
	// ����
	float speed = 1.0f;
#pragma region ���f��
	KModel triangle = Triangle();
	triangle.CreateModel(dxCommon->GetDev().Get());
	KModel cube = Cube();
	cube.CreateModel(dxCommon->GetDev().Get());
	KModel line = Line();
	line.CreateModel(dxCommon->GetDev().Get());
#pragma endregion
#pragma region �e�N�X�`��������
	const wchar_t* msg = L"Resources/mario.jpg";
	const wchar_t* msg2 = L"Resources/iijan.jpg";
	const wchar_t* msg3 = L"Resources/haikei.jpg";
	const wchar_t* msg4 = L"Resources/kitanai.jpg";
	KTexture texture(dxCommon->GetDev().Get(), msg, msg3);
	KTexture texture2(dxCommon->GetDev().Get(), msg2, msg4);
#pragma endregion

#pragma region �X�v���C�g�N���X�ǂݍ���
	std::unique_ptr<Sprite> sprite;
	sprite = std::make_unique<Sprite>();
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�
	// 3D�I�u�W�F�N�g�p�p�C�v���C������
	PipelineSet object3dPipelineSet = Create3DObjectGpipeline(dxCommon->GetDev().Get());
	// �X�v���C�g�p�p�C�v���C������
	PipelineSet spritePipelineSet = sprite->SpriteCreateGraphicsPipeline(dxCommon->GetDev().Get());
#pragma region 3D�I�u�W�F�N�g������
	const int ObjectNum = 2;
	const int LineNum = 6;
	// 3D�I�u�W�F�N�g
	KWorldTransform* object3d[ObjectNum];
	for (int i = 0; i < ObjectNum; i++) {
		object3d[i] = new KWorldTransform();
		object3d[i]->Initialize(dxCommon->GetDev().Get());
		if (i > 0) {
			object3d[i]->material->colorR = object3d[i]->material->colorG = object3d[i]->material->colorB = 1.0f;
		}
	}
	object3d[0]->SetModel(&cube);
	object3d[0]->SetTexture(&texture);
	object3d[1]->SetModel(&cube);
	object3d[1]->SetTexture(&texture2);
#pragma endregion
#pragma region �r���[
	// �r���[�v���W�F�N�V����
	ViewProjection* viewProjection;
	viewProjection = new ViewProjection(KWinApp::window_width, KWinApp::window_height);
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

	SoundData soundData1 = sound->SoundLoadWave("Sound/fanfare.wav");
#pragma region �X�v���C�g
	SpriteCommon spriteCommon;
	spriteCommon = sprite->SpriteCommonCreate(dxCommon->GetDev().Get(), KWinApp::window_width, KWinApp::window_height);

	sprite->SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/haikei.jpg", dxCommon->GetDev().Get());
	sprite->SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/mario.jpg", dxCommon->GetDev().Get());

	SpriteInfo sprites[2];
	for (int i = 0; i < _countof(sprites); i++)
	{
		sprites[i] = sprite->SpriteCreate(dxCommon->GetDev().Get(), KWinApp::window_width, KWinApp::window_height,
			sprites[i].texNum, spriteCommon);
		sprites[i].size.x = 100.0f;
		sprites[i].size.y = 100.0f;
		sprite->SpriteTransferVertexBuffer(sprites[i], spriteCommon);
	}
	sprites[0].texNum = 0;
	sprites[1].texNum = 1;
	sprites[1].rotation = 0;
	sprites[1].position.y = 720 / 2, 0;
#pragma endregion

#pragma region �f�o�b�O�e�L�X�g
	std::unique_ptr<DebugText> debugtext;
	debugtext = std::make_unique<DebugText>();

	const int debugTextNumber = 2;
	sprite->SpriteCommonLoadTexture(spriteCommon, debugTextNumber, L"Resources/tex1.png", dxCommon->GetDev().Get());
	debugtext->Init(dxCommon->GetDev().Get(), KWinApp::window_width, KWinApp::window_height, debugTextNumber, spriteCommon);
#pragma endregion
#pragma endregion

	// �E�B���h�E�\��
	// �Q�[�����[�v
	while (true)
	{
#pragma region �E�B���h�E���b�Z�[�W
		if (win->GetBreak() || input->IsPush(DIK_ESCAPE)) {
			break;
		}
#pragma endregion

		// �X�V
#pragma region ��Ղ̍X�V
		// �E�B���h�E�X�V
		win->Update();

		// input�X�V
		input->Update();
#pragma endregion

#pragma region �V�[���̍X�V
		for (int i = 0; i < _countof(sprites); i++)
		{
			sprite->SpriteUpdate(sprites[i], spriteCommon);
		}

		// �w�i�F�ς�
		if (input->IsTriger(DIK_SPACE)) {
			sound->SoundPlayWave(sound->GetxAudio().Get(), soundData1);
		}
		// �摜�F�ς�
		if (object3d[0]->material->colorR <= 0 || object3d[0]->material->colorR >= 1) {
			rSpeed *= -1;
		}
		if (object3d[0]->material->colorG <= 0 || object3d[0]->material->colorG >= 1) {
			gSpeed *= -1;
		}
		if (object3d[0]->material->colorB <= 0 || object3d[0]->material->colorB >= 1) {
			bSpeed *= -1;
		}
		if (object3d[0]->material->colorA <= 0 || object3d[0]->material->colorA >= 1) {
			aSpeed *= -1;
		}
		object3d[0]->material->colorR += rSpeed;
		object3d[0]->material->colorG += gSpeed;
		object3d[0]->material->colorB += bSpeed;
		if (input->IsPush(DIK_X)) {
			object3d[0]->material->colorA += aSpeed;
		}
		// �}�`�c��]
		if (input->IsPush(DIK_C) ||
			input->IsPush(DIK_V)) {
			if (input->IsPush(DIK_C)) {
				object3d[0]->transform.rot.z += 0.1f;
			}

			if (input->IsPush(DIK_V)) {
				object3d[0]->transform.rot.z -= 0.1f;
			}
		}
		////�J�����ړ�
		if (input->IsPush(DIK_D) || input->IsPush(DIK_A) ||
			input->IsPush(DIK_W) || input->IsPush(DIK_S)) {
			if (input->IsPush(DIK_D)) {
				viewProjection->angleX += XMConvertToRadians(1.0f);
			}
			else if (input->IsPush(DIK_A)) {
				viewProjection->angleX -= XMConvertToRadians(1.0f);
			}
			if (input->IsPush(DIK_W)) {
				viewProjection->angleY -= XMConvertToRadians(1.0f);
			}
			else if (input->IsPush(DIK_S)) {
				viewProjection->angleY += XMConvertToRadians(1.0f);
			}
			// angle���W�A��y����]
			viewProjection->eye.x = viewProjection->lenZ * sinf(viewProjection->angleX);
			viewProjection->eye.y = viewProjection->lenZ * sinf(viewProjection->angleY);
			viewProjection->eye.z = viewProjection->lenZ * cosf(viewProjection->angleX) * cosf(viewProjection->angleY);
		}
		// ����]
		if (input->IsPush(DIK_RIGHT) ||
			input->IsPush(DIK_LEFT)) {
			if (input->IsPush(DIK_RIGHT)) {
				object3d[0]->transform.rot.y -= 0.1f;
			}
			if (input->IsPush(DIK_LEFT)) {
				object3d[0]->transform.rot.y += 0.1f;
			}
		}
		// �O�x�N�g��
		object3d[0]->rotResult.x = sin(object3d[0]->transform.rot.y) * center.z;
		object3d[0]->rotResult.z = cos(object3d[0]->transform.rot.y) * center.z;
		// �ړ�
		/*object3d[0]->transform.pos.x += (speed)*object3d[0]->rotResult.x;
		object3d[0]->transform.pos.z += (speed)*object3d[0]->rotResult.z;*/
		object3d[0]->transform.pos.x += (input->IsPush(DIK_RIGHT) - input->IsPush(DIK_LEFT)) * speed;
		object3d[0]->transform.pos.z += (input->IsPush(DIK_UP) - input->IsPush(DIK_DOWN)) * speed;

		object3d[1]->transform.pos.x = object3d[0]->transform.pos.x + 15;
		object3d[1]->transform.pos.z = object3d[0]->transform.pos.z;
		object3d[1]->transform.pos.y = object3d[0]->transform.pos.y + 10;

		object3d[1]->transform.rot = object3d[0]->transform.rot;

		// �r���[�̃A�b�v�f�[�g
		viewProjection->Update(KWinApp::window_width, KWinApp::window_height);

		// 3D�I�u�W�F�N�g�̃A�b�v�f�[�g
		for (int i = 0; i < ObjectNum; i++) {
			object3d[i]->Update(viewProjection->matView, viewProjection->matProjection);
		}

#pragma endregion

#pragma region �`��
		// �`��J�n
		dxCommon->PreDraw();
#pragma region �p�C�v���C���X�e�[�g�ݒ�
		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		dxCommon->GetCmdlist().Get()->SetPipelineState(object3dPipelineSet.pipelineState.Get());
		dxCommon->GetCmdlist().Get()->SetGraphicsRootSignature(object3dPipelineSet.rootSignature.Get());
#pragma endregion
#pragma region �v���~�e�B�u�`��
		// �v���~�e�B�u�`��̐ݒ�R�}���h
		dxCommon->GetCmdlist().Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region �`��R�}���h
		// �`��R�}���h
		for (int i = 0; i < ObjectNum; i++) {
			if (!input->IsPush(DIK_SPACE))
			{
				object3d[i]->Draw(dxCommon->GetCmdlist().Get());
			}
			else
			{
				object3d[i]->SecoundDraw(dxCommon->GetCmdlist().Get());
			}
		}
		// �X�v���C�g�`��
		sprite->SpriteCommonBeginDraw(dxCommon->GetCmdlist().Get(), spriteCommon);
		for (int i = 0; i < _countof(sprites); i++)
		{
			sprite->SpriteDraw(sprites[i], dxCommon->GetCmdlist().Get(), spriteCommon, dxCommon->GetDev().Get());
		}

		debugtext->Print(spriteCommon, "Hello,DirectX!!", { 200,100 });
		debugtext->Print(spriteCommon, "Nihon Kogakuin", { 200,200 }, 2.0f);

		debugtext->DrawAll(dxCommon->GetDev().Get(), spriteCommon, dxCommon->GetCmdlist().Get());

		// �`��R�}���h�����܂�
#pragma endregion
		// �`��I��
		dxCommon->PostDraw();
	}
#pragma region ��Ղ̏I��
	sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);
	// ���͉��
	delete input;
	// 
	delete win;
	// 
	delete dxCommon;
#pragma endregion

	return 0;
}