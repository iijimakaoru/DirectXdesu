#include "EffectManager.h"

void EffectManager::Init(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	arrowModel_ = std::make_unique<MeshModel>("arrowEffect");

	for (size_t i = 0; i < maxArrowEffect; i++)
	{
		arrowEffect_[i] = std::make_unique<ArrowEffect>();
		arrowEffect_[i]->Init(arrowModel_.get(), timer, matView, matProjection);
	}
}

void EffectManager::Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	DeleteEffect();

	for (std::unique_ptr<ArrowEffect>& arrowEffect : arrowEffect_)
	{
		arrowEffect->Update(timer, matView, matProjection);
	}
}

void EffectManager::Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	for (std::unique_ptr<ArrowEffect>& arrowEffect : arrowEffect_)
	{
		arrowEffect->Draw(timer, matView, matProjection);
	}
}

void EffectManager::DeleteEffect()
{
	
}

void EffectManager::SetArrowEffect(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
	const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	
}
