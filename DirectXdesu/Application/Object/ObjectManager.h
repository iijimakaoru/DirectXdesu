#pragma once
#include "CenterObject.h"

class ObjectManager
{
public:
	// 初期化
	void Init(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	// 更新
	void Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	// 描画
	void Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

	// オブジェクト配置
	void SetCenterObject(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
		const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

private:
	std::unique_ptr<MeshModel> centerModel_;
	std::unique_ptr<CenterObject> centerObject_;
};

