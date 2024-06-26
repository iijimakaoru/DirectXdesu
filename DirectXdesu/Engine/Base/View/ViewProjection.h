#pragma once
#include "Vector3.h"
#include "KWinApp.h"
#include "MyMath.h"

/**
 * @file ViewProjection.h
 * @brief ビュープロジェクション
 * @author 飯島 薫
 */

class ViewProjection
{
public:
	// コンストラクタ
	ViewProjection();

	// 初期化
	void Initialize();

	// 更新
	void Update();

private:
	// カメラの距離
	float lenZ = 0;
	float nearZ = 0;
	float farZ = 0;
	// ビュー変換行列
	KMyMath::Matrix4 matView{};
	KMyMath::Vector3 eye{};
	KMyMath::Vector3 target{};
	KMyMath::Vector3 up{};
	// 射影変換行列の計算
	KMyMath::Matrix4 matProjection{};

	float aspect = 0; // アスペクト比

public:
	// ゲッター
	const KMyMath::Matrix4& GetMatView() const;
	const KMyMath::Matrix4& GetMatPro() const;
	const KMyMath::Vector3& GetEye() const;
	const KMyMath::Vector3& GetTarget() const;
	const KMyMath::Vector3& GetUp() const;

	// セッター
	void SetMatView(const KMyMath::Matrix4 mat_);
	void SetEye(const KMyMath::Vector3& eye_);
	void SetTarget(const KMyMath::Vector3& target_);
	void SetUp(const KMyMath::Vector3& up_);
	void SetFarZ(const float& farZ_);
	void SetNearZ(const float& nearZ_);
};

