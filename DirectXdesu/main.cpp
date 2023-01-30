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

//#include "DirectionLight.h"
#include "LightGroup.h"
#include <sstream>
#include <iomanip>

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
	//D3D12_ROOT_PARAMETER rootParam[3] = {};
	//// 定数バッファ0番
	//rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParam[0].Descriptor.ShaderRegister = 0;
	//rootParam[0].Descriptor.RegisterSpace = 0;
	//rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//// テクスチャレジスタ0番
	//rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//rootParam[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	//rootParam[1].DescriptorTable.NumDescriptorRanges = 1;
	//rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//// 定数バッファ1番
	//rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParam[2].Descriptor.ShaderRegister = 1;
	//rootParam[2].Descriptor.RegisterSpace = 0;
	//rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	D3D12_ROOT_PARAMETER rootParam[4] = {};
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

	rootParam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[3].Descriptor.ShaderRegister = 2;
	rootParam[3].Descriptor.RegisterSpace = 0;
	rootParam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

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

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	HRESULT result;
#pragma region 基盤初期化
#pragma region ウィンドウ
	KWinApp* win = nullptr;
	win = new KWinApp();
#pragma endregion
#pragma region DirectX初期化
	KDirectXCommon::Init(win);
	// キーボード入力
	KInput* input = nullptr;
	input = new KInput();
	input->Init(win);
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
	KModel sqhere = MtlObj("hoge");
	sqhere.CreateModel();
	KModel sqhere1 = MtlObj("hoge", true);
	sqhere1.CreateModel();
#pragma endregion
#pragma region テクスチャ初期化
	const wchar_t* msg = L"Resources/texture/mario.jpg";
	const wchar_t* msg2 = L"Resources/texture/iijan.jpg";
	const wchar_t* msg3 = L"Resources/texture/haikei.jpg";
	const wchar_t* msg4 = L"Resources/texture/kitanai.jpg";
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
#pragma region 3Dオブジェクト初期化
	const int ObjectNum = 2;
	const int LineNum = 6;
	// 3Dオブジェクト
	KObject3d* object3d[ObjectNum];
	for (int i = 0; i < ObjectNum; i++) {
		object3d[i] = new KObject3d();
		object3d[i]->Initialize();
		object3d[i]->transform.scale = { 1.0f,1,1 };
		if (i > 0) {
			object3d[i]->material->colorR = object3d[i]->material->colorG = object3d[i]->material->colorB = 1.0f;
		}
		object3d[i]->transform.scale = { 10,10,10 };
	}
	object3d[0]->LoadModel(&sqhere1);
	object3d[1]->LoadModel(&sqhere);
#pragma endregion
#pragma region ビュー
	// ビュープロジェクション
	ViewProjection viewProjection;
	viewProjection.Initialize(KWinApp::window_width, KWinApp::window_height);
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
	Sprite* sprite = new Sprite();
	sprite->Init(KDirectXCommon::GetInstance());

	SpriteCommon spriteCommon;
	spriteCommon = sprite->SpriteCommonCreate();

	sprite->SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/texture/haikei.jpg");
	sprite->SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/texture/mario.jpg");

	SpriteInfo sprites[2];
	for (int i = 0; i < _countof(sprites); i++)
	{
		sprites[i] = sprite->SpriteCreate(sprites[i].texNum, spriteCommon);
		sprites[i].size.x = 100.0f;
		sprites[i].size.y = 100.0f;
		sprite->SpriteTransferVertexBuffer(sprites[i], spriteCommon);
	}
	sprites[0].texNum = 0;
	sprites[1].texNum = 1;
	sprites[1].rotation = 0;
	sprites[1].position.y = 720 / 2, 0;
#pragma endregion

#pragma region デバッグテキスト
	DebugText* debugtext = new DebugText();

	const int debugTextNumber = 2;
	sprite->SpriteCommonLoadTexture(spriteCommon, debugTextNumber, L"Resources/texture/tex1.png");
	debugtext->Init(sprite, debugTextNumber, spriteCommon);
#pragma endregion
#pragma endregion

	bool isTexture = false;

	// ライト
	/*DirectionLight* light = nullptr;
	light = DirectionLight::Create();
	light->SetLightColor({ 1,1,1 });*/
	//KObject3d::SetLight(light);
	LightGroup* lightGroup = nullptr;
	lightGroup = LightGroup::Create();
	KObject3d::SetLightGroup(lightGroup);

	// imgui
	float ambientColor0[3] = { 1,1,1 };
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor[3] = { 0,0,1 };

	// ウィンドウ表示
	// ゲームループ
	while (true)
	{
#pragma region ウィンドウメッセージ
		if (win->ProcessMessage() || input->IsPush(DIK_ESCAPE))
		{
			break;
		}
#pragma endregion

		// 更新
#pragma region 基盤の更新
		// ウィンドウ更新

		// input更新
		input->Update();
#pragma endregion

#pragma region シーンの更新
		for (int i = 0; i < _countof(sprites); i++)
		{
			sprite->SpriteUpdate(sprites[i], spriteCommon);
		}

		// 背景色変え
		if (input->IsTriger(DIK_SPACE)) 
		{
			if (!isTexture)
			{
				isTexture = true;
			}
			else
			{
				isTexture = false;
			}
		}

		// 画像色変え
		if (object3d[0]->material->colorR <= 0 || object3d[0]->material->colorR >= 1) 
		{
			rSpeed *= -1;
		}
		if (object3d[0]->material->colorG <= 0 || object3d[0]->material->colorG >= 1) 
		{
			gSpeed *= -1;
		}
		if (object3d[0]->material->colorB <= 0 || object3d[0]->material->colorB >= 1) 
		{
			bSpeed *= -1;
		}
		if (object3d[0]->material->colorA <= 0 || object3d[0]->material->colorA >= 1) 
		{
			aSpeed *= -1;
		}
		object3d[0]->material->colorR += rSpeed;
		object3d[0]->material->colorG += gSpeed;
		object3d[0]->material->colorB += bSpeed;
		if (input->IsPush(DIK_X)) 
		{
			object3d[0]->material->colorA += aSpeed;
		}

		// 図形縦回転
		if (input->IsPush(DIK_C) ||
			input->IsPush(DIK_V))
		{
			if (input->IsPush(DIK_C))
			{
				object3d[0]->transform.rot.z += 0.1f;
			}

			if (input->IsPush(DIK_V))
			{
				object3d[0]->transform.rot.z -= 0.1f;
			}
		}

		//カメラ移動
		if (input->IsPush(DIK_D) || input->IsPush(DIK_A) ||
			input->IsPush(DIK_W) || input->IsPush(DIK_S))
		{
			if (input->IsPush(DIK_D))
			{
				viewProjection.angleX += XMConvertToRadians(1.0f);
			}
			else if (input->IsPush(DIK_A))
			{
				viewProjection.angleX -= XMConvertToRadians(1.0f);
			}
			if (input->IsPush(DIK_W))
			{
				viewProjection.angleY -= XMConvertToRadians(1.0f);
			}
			else if (input->IsPush(DIK_S))
			{
				viewProjection.angleY += XMConvertToRadians(1.0f);
			}
			// angleラジアンy軸回転
			viewProjection.eye.x = viewProjection.lenZ * sinf(viewProjection.angleX);
			viewProjection.eye.y = viewProjection.lenZ * sinf(viewProjection.angleY);
			viewProjection.eye.z = viewProjection.lenZ * cosf(viewProjection.angleX) * cosf(viewProjection.angleY);
		}

		// 横回転
		if (input->IsPush(DIK_RIGHT) ||
			input->IsPush(DIK_LEFT))
		{
			if (input->IsPush(DIK_RIGHT))
			{
				object3d[0]->transform.rot.y -= 0.1f;
			}
			if (input->IsPush(DIK_LEFT))
			{
				object3d[0]->transform.rot.y += 0.1f;
			}
		}

		// 前ベクトル
		object3d[0]->rotResult.x = sin(object3d[0]->transform.rot.y) * center.z;
		object3d[0]->rotResult.z = cos(object3d[0]->transform.rot.y) * center.z;

		// 移動
		/*object3d[0]->transform.pos.x += (speed)*object3d[0]->rotResult.x;
		object3d[0]->transform.pos.z += (speed)*object3d[0]->rotResult.z;*/
		/*object3d[0]->transform.pos.x += (input->IsPush(DIK_RIGHT) - input->IsPush(DIK_LEFT)) * speed;
		object3d[0]->transform.pos.z += (input->IsPush(DIK_UP) - input->IsPush(DIK_DOWN)) * speed;*/

		object3d[1]->transform.pos.x = object3d[0]->transform.pos.x + 20;
		object3d[1]->transform.pos.z = object3d[0]->transform.pos.z;
		object3d[1]->transform.pos.y = object3d[0]->transform.pos.y + 10;

		object3d[1]->transform.rot = object3d[0]->transform.rot;

		static XMVECTOR lightDir = { 0,1,5,0 };

		if (input->IsPush(DIK_I))
		{
			lightDir.m128_f32[1] += 1.0f;
		}
		else if (input->IsPush(DIK_K))
		{
			lightDir.m128_f32[1] -= 1.0f;
		}
		if (input->IsPush(DIK_L))
		{
			lightDir.m128_f32[0] += 1.0f;
		}
		else if (input->IsPush(DIK_J))
		{
			lightDir.m128_f32[0] -= 1.0f;
		}

		//light->SetLightDir(lightDir);

		std::ostringstream debugstr;
		debugstr << "lightDirFactor("
			<< std::fixed << std::setprecision(2)
			<< lightDir.m128_f32[0] << ","
			<< lightDir.m128_f32[1] << ","
			<< lightDir.m128_f32[2] << ")";
		debugtext->Print(spriteCommon, debugstr.str(), { 50, 50 }, 1.0f);
		debugstr.str("");
		debugstr.clear();

		//light->Update();
		lightGroup->Update();

		// ビューのアップデート
		viewProjection.Update(KWinApp::window_width, KWinApp::window_height);

		// 3Dオブジェクトのアップデート
		for (int i = 0; i < ObjectNum; i++) {
			object3d[i]->Update(viewProjection);
		}

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
		/*for (int i = 0; i < ObjectNum; i++)
		{
			object3d[i]->Draw();
		}*/
		if (!isTexture)
		{
			object3d[0]->Draw();
		}
		else
		{
			object3d[0]->Draw(&mario);
		}
		object3d[1]->Draw();

		// スプライト描画
		sprite->SpriteCommonBeginDraw(spriteCommon);
		/*for (int i = 0; i < _countof(sprites); i++)
		{
			sprite->SpriteDraw(sprites[i], spriteCommon);
		}*/

		if (!isTexture)
		{
			debugtext->Print(spriteCommon, "Textrue:mtl", { 10,10 }, 2.0f);
		}
		else
		{
			debugtext->Print(spriteCommon, "Textrue:tex", { 10,10 }, 2.0f);
		}
		//debugtext->Print(spriteCommon, "Nihon Kogakuin", { 200,200 }, 2.0f);
		debugtext->Print(spriteCommon, "FPS(w)" + std::to_string(KDirectXCommon::GetInstance()->fps), { 10,50 }, 2.0f);
		debugtext->DrawAll(spriteCommon);

		// 描画コマンドここまで
#pragma endregion
		// 描画終了
		KDirectXCommon::PostDraw();
	}

	delete sprite;
	//delete light;
	delete lightGroup;

#pragma region 基盤の終了
	sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);
	// 入力解放
	delete input;
	// DirectXCommon解放
	//delete dxCommon;
	// WindowsAPI終了処理
	win->Finalize();
	// WindowsAPI解放
	delete win;
	win = nullptr;
#pragma endregion

	return 0;
}