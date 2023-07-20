#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "FbxObject3D.h"

#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"

class TitleScene : public BaseScene
{
public:
	TitleScene(){};
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
	std::unique_ptr<KGPlin> spritePipeline = nullptr;

	// シェーダー
	KShader spriteShader;

	// タイトルスプライト
	/*std::unique_ptr<Sprite> titleName = nullptr;
	KTexture titleTex;*/
};

