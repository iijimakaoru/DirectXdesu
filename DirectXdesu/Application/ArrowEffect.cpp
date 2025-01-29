#include "ArrowEffect.h"

ArrowEffect* ArrowEffect::Create(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
	MeshModel* model, float& timeLimit, const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	// インスタンス生成
	ArrowEffect* instance = new ArrowEffect();
	if (instance == nullptr) {
		return nullptr;
	}

	instance->SetModel(model);
	instance->SetTimeLimit(timeLimit);
	instance->Init(pos, rotation, scale,color, timer, matView, matProjection);

	return instance;
}

void ArrowEffect::Init(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
	const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	emitter_ = std::make_unique<Emitter>(100, 1, 1.0f, 2.5f, 0.05f, 
		MyMathConvert::ChangeVector3toXMfloat3(pos),
		MyMathConvert::ChangeVector3toXMfloat3(scale), 
		MyMathConvert::ChangeVector3toXMfloat3(rotation), 
		MyMathConvert::ChangeXMFLOAT4(color), 
		MyMathConvert::ChangeXMFLOAT4(color),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	effect_ = std::make_unique<MeshGPUParticle>(timer, matView, matProjection, emitter_.get(), meshModel_);
}

void ArrowEffect::Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	effect_->Update(timer, matView, matProjection, emitter_.get());

	if (lifeTimer_ < timeLimit_) 
	{
		lifeTimer_++;
	}
	else 
	{
		isDead = true;
	}
}

void ArrowEffect::Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	effect_->Draw(timer, matView, matProjection, emitter_.get());
}

void ArrowEffect::SetModel(MeshModel* model)
{
	meshModel_ = model;
}

void ArrowEffect::SetTimeLimit(float& timeLimit)
{
	timeLimit_ = timeLimit;
}
