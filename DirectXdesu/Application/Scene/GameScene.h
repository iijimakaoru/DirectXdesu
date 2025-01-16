#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "FbxObject3D.h"
#include "KDirectXCommon.h"
#include "KGPlin.h"
#include "KInput.h"
#include "KModel.h"
#include "KObject3d.h"
#include "KShader.h"
#include "PostEffect.h"
#include "Sprite.h"
#include "Warning.h"
#include <array>
#include <imgui.h>

#include "AudioManager.h"

#include "Light.h"

#include "GameManager.h"
#include "CollisionManager.h"


#include "TitleCamera.h"
#include "GameCamera.h"

#include "MusicDesc.h"

/**
 * @file GameScene.h
 * @brief ゲームシーン
 * @author 飯島 薫
 */

class GameScene : public BaseScene {
public:
	enum Scene {
		Games = 0,
		Over = 1,
		Movies = 2
	};

public:
	GameScene(){};
	~GameScene();
	void LoadResources() override;
	void Init() override;
	void Update() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Final() override;

private:
	enum OBJ
	{
		stage,
		line,
		skydome,
		max,
	};

	enum
	{
		PERFECT,
		GREAT,
		MISS,
	};

private:
	//マウス角度算出
	void RotAndLenCalculationMouse();

	//スティック角度、長さ算出
	void RotAndLenCalculationStick(KMyMath::Vector2 vec);

	//当たり判定
	void Collision();

	//csv読み込み
	void LoadCSV(const std::string& name);

private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<GameCamera> camera = nullptr;

	// オブジェクト
	std::array<std::unique_ptr<KObject3d>, OBJ::max> obj;
	std::vector<std::unique_ptr<KObject3d>>objNote;

	// モデル
	std::array<KModel*, OBJ::max> objModel;
	KModel* noteModel;

	// オーディオ
	AudioManager* audioManager_;

	// ライト
	std::unique_ptr<Light> light_ = nullptr;
	KMyMath::Vector3 lightRGB_ = {1, 1, 1};
	KMyMath::Vector3 lightDir_ = {0, -1, 0};

	// 当たり判定マネージャー
	CollisionManager* collisionManager_ = nullptr;

private:
	//ノーツ
	std::unique_ptr<MusicDesc>music;
	std::vector<Note>notes;

	KMyMath::Vector2 start, end;
	float angle;
	float length;
	float lenRimit;
	float speed = 3.0f;
	float posZ = 0.1f;
	float playTime;
	float notePosZ;
	float sec = 0.1f;
	float minusShift = 10;
	float perfect = 2;
	float fovAngle;
	int32_t blankSpace = 0;
	const int32_t constblankSpace = 240;
	int score[3];
	int combo;
};
