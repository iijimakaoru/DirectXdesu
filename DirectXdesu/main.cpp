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
#include "DebugCamera.h"
#include "Camera.h"
#include "MyMath.h"
#include "ParticleManager.h"

#include "KGPlin.h"

PipelineSet Create3DObjectGpipeline()
{
	HRESULT result;
#pragma region シェーダー読み込みとコンパイル
	std::unique_ptr<KShader> shader;
	shader = std::make_unique<KShader>();
	shader->ObjVSLoadCompile();
	shader->ObjPSLoadCompile();
#pragma endregion
#pragma region 頂点レイアウト配列の宣言と設定
	static D3D12_INPUT_ELEMENT_DESC inputLayout[3] = {
	{// xyz座標
		"POSITION",										// セマンティック名
		0,												// 同じセマンティック名が複数あるときに使うインデックス
		DXGI_FORMAT_R32G32B32_FLOAT,					// 要素数とビット数を表す
		0,												// 入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,					// データのオフセット
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// 入力データ種別
		0												// 一度に描画するインスタンス数
	},
	{// 法線ベクトル
		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	{// uv座標
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	};
#pragma endregion
#pragma region パイプラインステート設定変数の宣言と各種項目の設定
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	// シェーダーの設定
	gpipeline.VS.pShaderBytecode = shader->GetVSBlob()->GetBufferPointer();
	gpipeline.VS.BytecodeLength = shader->GetVSBlob()->GetBufferSize();
	gpipeline.PS.pShaderBytecode = shader->GetPSBlob()->GetBufferPointer();
	gpipeline.PS.BytecodeLength = shader->GetPSBlob()->GetBufferSize();
	// サンプルマスクの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザの設定
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面をカリング
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;

	// ブレンドステート
	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpipeline.SampleDesc.Count = 1;

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// 半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParam[3] = {};
	// 定数バッファ0番
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[0].Descriptor.ShaderRegister = 0;
	rootParam[0].Descriptor.RegisterSpace = 0;
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// テクスチャレジスタ0番
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// 定数バッファ1番
	rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[2].Descriptor.ShaderRegister = 1;
	rootParam[2].Descriptor.RegisterSpace = 0;
	rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// テクスチャサンブラーの設定
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
	// パイプラインとルートシグネチャのセット
	PipelineSet pipelineSet;
#pragma region ルートシグネチャの生成
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = _countof(rootParam);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3D10Blob> errBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, errBlob.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result));
	result = KDirectXCommon::GetInstance()->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootSignature));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = pipelineSet.rootSignature.Get();
#pragma endregion
#pragma region グラフィックスパイプラインステートの生成
	// グラフィックスパイプラインステートの設定
	gpipeline.DepthStencilState.DepthEnable = true; // 深度テスト
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 書き込み許可
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さければ合格
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット

	result = KDirectXCommon::GetInstance()->GetDev()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

	return pipelineSet;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#pragma region 基盤初期化

#pragma region ウィンドウ
	KWinApp::Init();
#pragma endregion

#pragma region DirectX初期化
	KDirectXCommon::GetInstance()->Init();
	// キーボード入力
	KInput::Init();
#pragma endregion
#pragma endregion

#pragma region シーンの初期化
	// 速さ
	float speed = 1.0f;

#pragma region モデル
	std::unique_ptr<KModel> triangle = std::make_unique<Triangle>();
	triangle->CreateModel();
	std::unique_ptr<KModel> cube = std::make_unique<Cube>();
	cube->CreateModel();
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
	KModel sphere = MtlObj("sphere");
	sphere.CreateModel();
#pragma endregion

#pragma region テクスチャ初期化
	KTexture mario;
	mario.CreateTexture("Resources/texture/", "mario.jpg");
	KTexture kitanai;
	kitanai.CreateTexture("Resources/texture/", "kitanai.jpg");
	KTexture haikei;
	haikei.CreateTexture("Resources/texture/", "haikei.jpg");
	KTexture stageR;
	stageR.CreateTexture("Resources/texture/", "stageHoge.png");
#pragma endregion

#pragma region グラフィックスパイプライン設定
	// 3Dオブジェクト用パイプライン生成
	PipelineSet object3dPipelineSet = Create3DObjectGpipeline();
	KShader shader;
	shader.Init(L"ObjVS.hlsl", L"ObjPS.hlsl");

	std::unique_ptr<KGPlin> pipeline;
	pipeline = std::make_unique<KGPlin>(shader);

	// プレイヤー
	Player player(cube.get());

	Player::nowPlayer = &player;

	if (!ParticleManager::GetInstance()->IsPoolCreated())
	{
		ParticleManager::GetInstance()->CreatePool(cube.get());
	}

#pragma region ビュー
	// ビュープロジェクション
	ViewProjection viewProjection;
	viewProjection.Initialize();
	viewProjection.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();
#pragma endregion

#pragma endregion
	std::unique_ptr<Sound> sound;
	sound = std::make_unique<Sound>();
	sound->Init();

	SoundData soundData1 = sound->SoundLoadWave("Resources/Sound/fanfare.wav");
#pragma region スプライト
	Sprite sprite;
	sprite.Init(KDirectXCommon::GetInstance());

	SpriteCommon spriteCommon;
	spriteCommon = sprite.SpriteCommonCreate();
#pragma endregion

#pragma region デバッグテキスト
	DebugText* debugtext = new DebugText();

	const int debugTextNumber = 4;
	sprite.SpriteCommonLoadTexture(spriteCommon, debugTextNumber, L"Resources/texture/tex1.png");
	debugtext->Init(&sprite, debugTextNumber, spriteCommon);
#pragma endregion
#pragma endregion

	bool isTexture = false;

	int phase = 1;

	float phaseTimer = 0;

	Vector3 vec = { 1,1,1 };

	float angle = 0;

	float hogeSpeed = 0;

	float speedLevel = 1;

	float hogeLifeTime = 0;

	float hogeAngle = 0;

	float hogeRot = 0;

	float hogeCooltime = 0;

	// ウィンドウ表示
	// ゲームループ
	while (true)
	{
#pragma region ウィンドウメッセージ
		if (KWinApp::GetInstance()->ProcessMessage() || KInput::GetInstance()->IsPush(DIK_ESCAPE))
		{
			break;
		}
#pragma endregion

		// 更新
#pragma region 基盤の更新
		// input更新
		KInput::Update();
#pragma endregion

#pragma region シーンの更新
		float piAngle = PI * 2;

		if (KInput::GetInstance()->IsTriger(DIK_SPACE))
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

		if (KInput::GetInstance()->IsTriger(DIK_K))
		{
			if (speedLevel > 1)
			{
				speedLevel--;
			}
		}

		if (KInput::GetInstance()->IsTriger(DIK_L))
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

		if (KInput::GetInstance()->IsPress(DIK_W))
		{
			viewProjection.angleY -= XMConvertToRadians(1.0f);
		}
		if (KInput::GetInstance()->IsPress(DIK_S))
		{
			viewProjection.angleY += XMConvertToRadians(1.0f);
		}
		if (KInput::GetInstance()->IsPress(DIK_A))
		{
			viewProjection.angleX -= XMConvertToRadians(1.0f);
		}
		if (KInput::GetInstance()->IsPress(DIK_D))
		{
			viewProjection.angleX += XMConvertToRadians(1.0f);
		}

		viewProjection.eye.x = viewProjection.lenZ * cosf(viewProjection.angleX) * cosf(viewProjection.angleY);
		viewProjection.eye.y = (viewProjection.lenZ * sinf(viewProjection.angleY));
		viewProjection.eye.z = viewProjection.lenZ * sinf(viewProjection.angleX) * cosf(viewProjection.angleY);

		// プレイヤー初期化
		player.Init();

		ParticleManager::GetInstance()->Update(viewProjection);

		// ビューのアップデート
		viewProjection.Update();
#pragma endregion

#pragma region 描画
		// 描画開始
		KDirectXCommon::GetInstance()->PreDraw();
#pragma region パイプラインステート設定
		// パイプラインステートとルートシグネチャの設定コマンド
		KDirectXCommon::GetInstance()->GetCmdlist()->SetPipelineState(object3dPipelineSet.pipelineState.Get());
		KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootSignature(object3dPipelineSet.rootSignature.Get());
#pragma endregion
#pragma region プリミティブ形状
		// プリミティブ形状の設定コマンド
		KDirectXCommon::GetInstance()->GetCmdlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region 描画コマンド

		ParticleManager::GetInstance()->Draw();

		// スプライト描画
		sprite.SpriteCommonBeginDraw(spriteCommon);

		debugtext->Print(spriteCommon, "FPS(w)" + std::to_string(KDirectXCommon::GetInstance()->fps), { 10,50 }, 2.0f);
		debugtext->DrawAll(spriteCommon);

		// 描画コマンドここまで
#pragma endregion
		// 描画終了
		KDirectXCommon::GetInstance()->PostDraw();
	}

#pragma region 基盤の終了
	sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);
#pragma endregion

	return 0;
}