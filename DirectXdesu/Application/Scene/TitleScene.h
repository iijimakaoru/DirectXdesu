#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "FbxObject3D.h"

#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "KTexture.h"

#include "Sprite.h"

class TitleScene : public BaseScene
{
public:
	TitleScene()
	{
		LoadResources();
		Init();
	};
	~TitleScene();
	void LoadResources()override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Final()override;

private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> fbxPipeline;
	std::unique_ptr<KGPlin> spritePipeline;

	// シェーダー
	KShader fbxShader;
	KShader spriteShader;

	// Fbx関連
	FbxModel* fbxModel1 = nullptr;
	FbxObject3D* object1 = nullptr;

	// ポストエフェクト
	std::unique_ptr<PostEffect> postEffect = nullptr;

	// スプライト
	Sprite* sprite = nullptr;
	KMyMath::Vector2 spritePos = { 0,0 };
	KMyMath::Vector2 spriteSize = { 100,100 };
	float spriteRot = 0;
	KMyMath::Vector4 spriteColor = { 1.0f,1.0f,1.0f,1.0f };
	bool spriteFlipX, spriteFlipY = false;

	// テクスチャ
	KTexture tex1;
};

