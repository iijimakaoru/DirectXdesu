#include "GameScence.h"

void GameScence::Init(ID3D12Device& dev, int window_width, int window_height)
{
	// モデル
	triangle.CreateModel(dx->GetDev().Get());
	cube.CreateModel(dx->GetDev().Get());
	line.CreateModel(dx->GetDev().Get());
	// テクスチャ初期化
	const wchar_t* msg = L"Resources/mario.jpg";
	const wchar_t* msg2 = L"Resources/iijan.jpg";
	const wchar_t* msg3 = L"Resources/haikei.jpg";
	const wchar_t* msg4 = L"Resources/kitanai.jpg";
	KTexture texture(dx->GetDev().Get(), msg, msg3);
	KTexture texture2(dx->GetDev().Get(), msg2, msg4);
	// スプライト
	sprite = std::make_unique<Sprite>();
	// 3Dオブジェクト
	for (int i = 0; i < ObjectNum; i++) 
	{
		object3d[i] = new KWorldTransform();
		object3d[i]->Initialize(dx->GetDev().Get());
		if (i > 0) 
		{
			object3d[i]->material->colorR =
				object3d[i]->material->colorG =
				object3d[i]->material->colorB = 1.0f;
		}
	}
	object3d[0]->SetModel(&cube);
	object3d[0]->SetTexture(&texture);
	object3d[1]->SetModel(&cube);
	object3d[1]->SetTexture(&texture2);
	// ビュープロジェクション
	viewProjection = new ViewProjection(window_width, window_height);
	// サウンド
	sound = std::make_unique<Sound>();
	sound->Init();

	SoundData soundData1 = sound->SoundLoadWave("Sound/fanfare.wav");
	// スプライト
	spriteCommon = sprite->SpriteCommonCreate(dx->GetDev().Get(), window_width, window_height);

	sprite->SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/haikei.jpg", dx->GetDev().Get());
	sprite->SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/mario.jpg", dx->GetDev().Get());

	for (int i = 0; i < _countof(sprites); i++)
	{
		sprites[i] = sprite->SpriteCreate(dx->GetDev().Get(), window_width, window_height,
			sprites[i].texNum, spriteCommon);
		sprites[i].size.x = 100.0f;
		sprites[i].size.y = 100.0f;
		sprite->SpriteTransferVertexBuffer(sprites[i], spriteCommon);
	}
	sprites[0].texNum = 0;
	sprites[1].texNum = 1;
	sprites[1].rotation = 0;
	sprites[1].position.y = 720 / 2, 0;
	//
	debugtext = std::make_unique<DebugText>();

	const int debugTextNumber = 2;
	sprite->SpriteCommonLoadTexture(spriteCommon, debugTextNumber, L"Resources/tex1.png", dx->GetDev().Get());
	debugtext->Init(dx->GetDev().Get(), window_width, window_height, debugTextNumber, spriteCommon);
}

void GameScence::Update()
{
}

void GameScence::Draw(ID3D12Device& dev, ID3D12GraphicsCommandList* cmdList)
{
}
