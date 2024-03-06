#pragma once
#include "MyMath.h"

class Transform {
public:
	void TransUpdate();

	// ゲッター
	const KMyMath::Vector3& GetPos() const;
	const KMyMath::Vector3& GetRot() const;
	const KMyMath::Vector3& GetScale() const;
	const KMyMath::Matrix4& GetMatWorld() const;
	const KMyMath::Vector3 GetWorldPos() const;
	const Transform* GetParent() const;

	// セッター
	void SetPos(const KMyMath::Vector3& pos_);
	void SetRot(const KMyMath::Vector3& rot_);
	void SetScale(const KMyMath::Vector3& scale_);
	void SetMatWorld(const KMyMath::Matrix4& matWorld_);
	void SetParent(const Transform* parent_);
	void AddSetPos(const KMyMath::Vector3& pos_);
	void AddSetRot(const KMyMath::Vector3& rot_);
	void AddSetScale(const KMyMath::Vector3& scale_);

private:
	// アフィン変換
	KMyMath::Vector3 scale = {1, 1, 1};
	KMyMath::Vector3 rot = {0, 0, 0};
	KMyMath::Vector3 pos = {0, 0, 0};

	// ワールド変換行列
	KMyMath::Matrix4 matWorld = {};

	// 親オブジェクトへのポインタ
	const Transform* parent = nullptr;
};
