#pragma once
#include "Camera.h"
#include "KGPlin.h"

#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"
#include "TitleCamera.h"

/**
 * @file TitleScene.h
 * @brief タイトルシーン
 * @author 飯島 薫
 */

class TitleScene : public BaseScene
{
public:
	TitleScene(){};
	~TitleScene();
	void LoadResources()override;
	void Init() override;
	void Update() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Final()override;

	// タイトル導入演出
	void StartScene();

	// 次のシーンへ
	void GoNextScene();

	// タイトルコール
	void TitleCall();
private:
	std::unique_ptr<KModel> model_;
	std::unique_ptr<KObject3d> object_;
};

