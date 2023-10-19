#pragma once
#include "ViewProjection.h"
#include "KWinApp.h"
#include <memory>
#include "KObject3d.h"

/**
 * @file Camera.h
 * @brief カメラの基底クラス
 * @author 飯島 薫
 */

class Camera
{
protected:
	std::unique_ptr<ViewProjection> viewProjection = nullptr;

	std::unique_ptr<KObject3d> cameraObject = nullptr;

public:
	virtual ~Camera() {}

	// 初期化
	virtual void Init();

	// 更新
	virtual void Update();

	// ビュープロジェクションゲッター
	ViewProjection* GetViewPro();

	// ゲッター
	const WorldTransfom& GetTransform()const;
	const KMyMath::Vector3& GetPos()const;
	const KMyMath::Vector3 GetWorldPos() const;

	// セッター
	void SetPos(const KMyMath::Vector3 pos);
};