#include "SideObjectEffect.h"
#include "Ease.h"

void SideObjectEffect::Init(MeshModel* model, const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
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

void SideObjectEffect::Update(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	if (!isDead)
	{
		KMyMath::Vector3 easeS = MyEase::OutCubicVec3(startScale_, endScale_, lifeTimer_ / lifeLimit_);
		DirectX::XMFLOAT3 easeScale = MyMathConvert::ChangeVector3toXMfloat3(easeS);
		float particleSize = MyEase::Lerp(1.5f, 0.0f, lifeTimer_ / lifeLimit_);

		emitter_->SetScaling(easeScale);
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

void SideObjectEffect::Draw(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	if (!isDead)
	{
		effect_->Draw(timer, matView, matProjection, emitter_.get());
	}
}

bool SideObjectEffect::GetIsDead()
{
	return isDead;
}

void SideObjectEffect::SetParticle(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color, const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	DirectX::XMFLOAT3 nowPos = MyMathConvert::ChangeVector3toXMfloat3(pos);
	DirectX::XMFLOAT3 nowRot = MyMathConvert::ChangeVector3toXMfloat3(rotation);
	DirectX::XMFLOAT3 nowScale = MyMathConvert::ChangeVector3toXMfloat3(scale);
	DirectX::XMFLOAT4 nowColor = MyMathConvert::ChangeXMFLOAT4(color);
	float nowParticleSize = 2.0f;

	// 仮
	float particlePower = 4.0f;
	startScale_ = scale * 2.0f;
	endScale_ = scale * particlePower;

	emitter_->SetPosition(nowPos);
	emitter_->SetScaling(nowScale);
	emitter_->SetRotation(nowRot);
	emitter_->SetStartColor(nowColor);
	emitter_->SetParticleSize(nowParticleSize);

	lifeTimer_ = 0.0f;

	isDead = false;
}
