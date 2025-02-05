#pragma once
#include <list>
#include "ArrowEffect.h"
#include "GroundEffect/GroundEffect.h"

const size_t maxEffectNum = 15;

class EffectSetter
{
public:
	// 初期化
	void Init(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	// 更新
	void Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	// 描画
	void Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

	// エフェクト設置
	void SetArrowEffect(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
		const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	void SetGroundEffect(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
		const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

private:
	void DeleteEffect();

private:
	std::unique_ptr<MeshModel> arrowModel_;
	//std::array<std::unique_ptr<ArrowEffect>, maxEffectNum> arrowEffect_;
	std::list<std::unique_ptr<ArrowEffect>> arrowEffect_;

	std::unique_ptr<MeshModel> groundModel_;
	//std::array<std::unique_ptr<GroundEffect>, maxEffectNum> groundEffect_;
	std::list<std::unique_ptr<GroundEffect>> groundEffect_;
};

