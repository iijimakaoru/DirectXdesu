#pragma once
#include "ViewProjection.h"
#include "KWinApp.h"
#include <memory>
#include "Transform.h"

/**
 * @file Camera.h
 * @brief カメラの基底クラス
 * @author 飯島 薫
 */

class Camera
{
protected:
	std::unique_ptr<ViewProjection> viewProjection = nullptr;

	KMyMath::Matrix4 matView{};

	Transform cameraTransform;

public:
	virtual ~Camera() {}

	// 初期化
	virtual void Init();

	// 更新
	virtual void Update();

	void ViewUpdate();

	// ビュープロジェクションゲッター
	ViewProjection* GetViewPro();

	// ゲッター
	const KMyMath::Matrix4& GetMatView() const;
	const Transform& GetTransform()const;
	const KMyMath::Vector3& GetCameraPos()const;
	const KMyMath::Vector3& GetCameraRot() const;
	const KMyMath::Vector3 GetWorldPos() const;

	// セッター
	void SetCameraPos(const KMyMath::Vector3& pos_);
	void SetCameraRot(const KMyMath::Vector3& rot_);
};