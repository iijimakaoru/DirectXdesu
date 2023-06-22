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
};

