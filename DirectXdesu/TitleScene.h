#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "FbxObject3D.h"

#include "BaseScene.h"

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
	// カメラ
	std::unique_ptr<Camera> camera = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> fbxPipeline;

	// シェーダー
	KShader fbxShader;

	// Fbx関連
	FbxModel* fbxModel1 = nullptr;
	FbxObject3D* object1 = nullptr;
};

