#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KTexture.h"
#include "KGPlin.h"
#include "KShader.h"
#include "KInput.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	// 初期化
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera_"></param>
	void Update(ViewProjection& viewPro);

	// 移動
	void Move();

	// 回転
	void Rot();

	// 描画
	void Draw();

private:
	// Input
	KInput* input = nullptr;
	// パッドの入力情報
	KMyMath::Vector2 leftStickPos;

	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	std::unique_ptr<KModel> model;

	// テクスチャ
	KTexture tex;

	// パイプライン
	std::unique_ptr<KGPlin> pipeline;

	// シェーダー
	KShader shader;

	// スピード
	float speed = 1.0f;

	// 回転戻すイージング用
	KMyMath::Vector2 oldRot;
	float rotEaseTimer = 0;
	bool isRotEase = false;

	// 動くスピード
	float moveSpeed = 0.5f;

	// 角度上限
	const KMyMath::Vector2 rotLimit = { 35.0f, 25.0f };
};