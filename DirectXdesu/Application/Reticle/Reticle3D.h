#pragma once
#include "KObject3d.h"
#include "KGPlin.h"
#include "KShader.h"

/**
 * @file Reticle3D.h
 * @brief レティクル3D座標
 * @author 飯島 薫
 */

class Reticle3D
{
public:
	// 初期化
	void Init();

	// 更新
	void Update(KMyMath::Matrix4& m, KMyMath::Vector3 pPos);

	// ゲッター
	KMyMath::Vector3 GetWorldPos();

private:
	std::unique_ptr<KObject3d> object = nullptr;
};