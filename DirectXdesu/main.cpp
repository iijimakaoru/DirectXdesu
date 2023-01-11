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
	gpipeline.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = shader->psBlob->GetBufferSize();
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
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader->errorBlob);
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

bool BoxCollision(WorldTransfom& transformA, WorldTransfom& transformB);

void AllCollision();

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#pragma region 基盤初期化
#pragma region ウィンドウ
	KWinApp::Init();
#pragma endregion
#pragma region DirectX初期化
	KDirectXCommon::Init();
	// キーボード入力
	KInput::Init();
#pragma endregion
#pragma endregion

#pragma region シーンの初期化
	// 速さ
	float speed = 1.0f;
#pragma region モデル
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
#pragma region テクスチャ初期化
	KTexture mario;
	mario.CreateTexture("Resources/texture/", "mario.jpg");
	KTexture kitanai;
	kitanai.CreateTexture("Resources/texture/", "kitanai.jpg");
	KTexture haikei;
	haikei.CreateTexture("Resources/texture/", "haikei.jpg");
#pragma endregion

#pragma region グラフィックスパイプライン設定
	// 3Dオブジェクト用パイプライン生成
	PipelineSet object3dPipelineSet = Create3DObjectGpipeline();

	// プレイヤー
	Player player(&cube);

	Player::nowPlayer = &player;

	// ボス
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

	// ステージ
	Stage stage;
	stage.Init(&cube);

	// 天球
	KObject3d boxSkydorm;
	boxSkydorm.Initialize();
	boxSkydorm.LoadModel(&boxSky);

	boxSkydorm.transform.scale = { 320,320,320 };

#pragma region ビュー
	// ビュープロジェクション
	ViewProjection viewProjection;
	viewProjection.Initialize();
	viewProjection.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();

	//// カメラ
	//Camera camera;

	//// デバッグカメラ
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
#pragma region スプライト
	Sprite sprite;
	sprite.Init(KDirectXCommon::GetInstance());

	SpriteCommon spriteCommon;
	spriteCommon = sprite.SpriteCommonCreate();

	sprite.SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/texture/playerColor.png");
	sprite.SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/texture/bossColor.png");

	// プレイヤーHPのUI
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

	// ボスHPのUI
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

#pragma region デバッグテキスト
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
			// プレイヤー初期化
			player.Init();

			// ボス初期化
			boss.Init();

			// 出てる弾を全て消す
			BossBulletManager::GetInstance()->AllDelete();

			// 出てるパーティクルを全て消す
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

		// ビューのアップデート
		viewProjection.Update();

		boxSkydorm.Update(Player::nowPlayer->view);
#pragma endregion

#pragma region 描画
		// 描画開始
		KDirectXCommon::PreDraw();
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
		// 描画コマンド
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

		// スプライト描画
		sprite.SpriteCommonBeginDraw(spriteCommon);
		if (gameScene == Scene::Play)
		{
#pragma region プレイヤーのHP
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

#pragma region ボスのHP
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

		// 描画コマンドここまで
#pragma endregion
		// 描画終了
		KDirectXCommon::PostDraw();
	}

#pragma region 基盤の終了
	sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);
#pragma endregion

	return 0;
}

bool BoxCollision(WorldTransfom& transformA, WorldTransfom& transformB)
{
	if (transformA.pos.x - transformA.scale.x <= transformB.pos.x + transformB.scale.x && // posA左とposB右の判定
		transformA.pos.x + transformA.scale.x >= transformB.pos.x - transformB.scale.x && // posA右とposB左の判定
		transformA.pos.y - transformA.scale.y <= transformB.pos.y + transformB.scale.y && // posA下とposB上の判定
		transformA.pos.y + transformA.scale.y >= transformB.pos.y - transformB.scale.y && // posA上とposB下の判定
		transformA.pos.z - transformA.scale.z <= transformB.pos.z + transformB.scale.z && // posA前とposB奥の判定
		transformA.pos.z + transformA.scale.z >= transformB.pos.z - transformB.scale.z)   // posA奥とposB前の判定
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