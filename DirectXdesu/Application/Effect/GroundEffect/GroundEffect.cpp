#include "GroundEffect.h"
#include "Ease.h"

void GroundEffect::Init(MeshModel* model, const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	lifeLimit_ = 10.0f;
	meshModel_ = model;
	isDead = true;
	emitter_ = std::make_unique<Emitter>(100, 1, 1.0f, 2.5f, 0.5f,
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	effect_ = std::make_unique<GroundEffectParticle>(timer, matView, matProjection, emitter_.get(), meshModel_);
}

void GroundEffect::Update(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	if (!isDead)
	{
		KMyMath::Vector3 ease = MyEase::OutCubicVec3(startPos_, endPos_, lifeTimer_ / lifeLimit_);
		DirectX::XMFLOAT3 easePos = MyMathConvert::ChangeVector3toXMfloat3(ease);
		float particleSize = MyEase::Lerp(2.0f, 0.0f, lifeTimer_ / lifeLimit_);

		emitter_->SetPosition(easePos);
		emitter_->SetParticleSize(particleSize);

		effect_->Update(timer, matView, matProjection, emitter_.get());

		if (lifeTimer_ < lifeLimit_)
		{
			lifeTimer_++;
		}
		else
		{
			isDead = true;
		}
	}
}

void GroundEffect::Draw(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	if (!isDead)
	{
		effect_->Draw(timer, matView, matProjection, emitter_.get());
	}
}

bool GroundEffect::GetIsDead()
{
	return isDead;
}

void GroundEffect::SetParticle(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
	const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	DirectX::XMFLOAT3 nowPos = MyMathConvert::ChangeVector3toXMfloat3(pos);
	DirectX::XMFLOAT3 nowRot = MyMathConvert::ChangeVector3toXMfloat3(rotation);
	DirectX::XMFLOAT3 nowScale = MyMathConvert::ChangeVector3toXMfloat3(scale);
	DirectX::XMFLOAT4 nowColor = MyMathConvert::ChangeXMFLOAT4(color);
	float nowParticleSize = 2.0f;

	// 仮
	float particlePower = 15.0f;
	startPos_ = pos;
	endPos_ = { pos.x,pos.y + particlePower,pos.z };

	emitter_->SetPosition(nowPos);
	emitter_->SetScaling(nowScale);
	emitter_->SetRotation(nowRot);
	emitter_->SetStartColor(nowColor);
	emitter_->SetParticleSize(nowParticleSize);

	lifeTimer_ = 0.0f;

	isDead = false;
}
