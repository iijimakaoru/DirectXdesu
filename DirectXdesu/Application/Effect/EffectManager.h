#pragma once
#include "ArrowEffect.h"

const size_t maxArrowEffect = 5;

class EffectManager
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

private:
	void DeleteEffect();

private:
	std::unique_ptr<MeshModel> arrowModel_;

	//std::list<std::unique_ptr<ArrowEffect>> arrowEffect_;
	std::array<std::unique_ptr<ArrowEffect>, maxArrowEffect> arrowEffect_;
	//std::unique_ptr<ArrowEffect> arrowEffect_;
};

