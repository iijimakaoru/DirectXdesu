#include "EffectSetter.h"

void EffectSetter::Init(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	arrowModel_ = std::make_unique<MeshModel>("arrowEffect");

	for (size_t i = 0; i < maxEffectNum; i++)
	{
		arrowEffect_[i] = std::make_unique<ArrowEffect>();
		arrowEffect_[i]->Init(arrowModel_.get(), timer, matView, matProjection);
	}

	groundModel_ = std::make_unique<MeshModel>("GroundEffect");

	for (size_t i = 0; i < maxEffectNum; i++)
	{
		groundEffect_[i] = std::make_unique<GroundEffect>();
		groundEffect_[i]->Init(groundModel_.get(), timer, matView, matProjection);
	}
}

void EffectSetter::Update(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	DeleteEffect();

	for (std::unique_ptr<ArrowEffect>& arrowEffect : arrowEffect_)
	{
		arrowEffect->Update(timer, matView, matProjection);
	}

	for (std::unique_ptr<GroundEffect>& groundEffect : groundEffect_)
	{
		groundEffect->Update(timer, matView, matProjection);
	}
}

void EffectSetter::Draw(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	for (std::unique_ptr<ArrowEffect>& arrowEffect : arrowEffect_)
	{
		arrowEffect->Draw(timer, matView, matProjection);
	}

	for (std::unique_ptr<GroundEffect>& groundEffect : groundEffect_)
	{
		groundEffect->Draw(timer, matView, matProjection);
	}
}

void EffectSetter::DeleteEffect()
{
	//arrowEffect_.remove_if([](std::unique_ptr<ArrowEffect>& arrowEffect) { return arrowEffect->GetIsDead(); });

	//groundEffect_.remove_if([](std::unique_ptr<GroundEffect>& groundEffect) { return groundEffect->GetIsDead(); });
}

void EffectSetter::SetArrowEffect(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
	const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	for (size_t i = 0; i < maxEffectNum; i++)
	{
		if (arrowEffect_[i]->GetIsDead())
		{
			arrowEffect_[i]->SetParticle(pos, rotation, scale, color, timer, matView, matProjection);
			break;
		}
	}

	/*std::unique_ptr<ArrowEffect> newArrowEffect = std::make_unique<ArrowEffect>();
	newArrowEffect->Init(arrowModel_.get(), timer, matView, matProjection);
	newArrowEffect->SetParticle(pos, rotation, scale, color, timer, matView, matProjection);

	arrowEffect_.push_back(std::move(newArrowEffect));*/
}

void EffectSetter::SetGroundEffect(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
	const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	for (size_t i = 0; i < maxEffectNum; i++)
	{
		if (groundEffect_[i]->GetIsDead())
		{
			groundEffect_[i]->SetParticle(pos, rotation, scale, color, timer, matView, matProjection);
			break;
		}
	}

	/*std::unique_ptr<GroundEffect> newGroundEffect = std::make_unique<GroundEffect>();
	newGroundEffect->Init(groundModel_.get(), timer, matView, matProjection);
	newGroundEffect->SetParticle(pos, rotation, scale, color, timer, matView, matProjection);

	groundEffect_.push_back(std::move(newGroundEffect));*/
}
