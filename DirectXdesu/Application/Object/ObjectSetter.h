#pragma once
#include "CenterObject.h"
#include "SideObject.h"

const size_t maxObjectNum = 20;

class ObjectSetter
{
public:
	// 初期化
	void Init(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	// 更新
	void Update(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	// 描画
	void Draw(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

	// オブジェクト配置
	void SetCenterObject(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color);
	void SetSideObject(size_t& i, KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color);

	void SetSideVibration();

private:
	std::unique_ptr<MeshModel> centerModel_;
	std::unique_ptr<CenterObject> centerObject_;

	std::unique_ptr<MeshModel> sideModel_;
	std::array<std::unique_ptr<SideObject>,maxObjectNum> leftObjects_;
	std::array<std::unique_ptr<SideObject>, maxObjectNum> rightObjects_;

	float leftSide_;
	float rightSide_;
};

