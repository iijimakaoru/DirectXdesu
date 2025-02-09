#include "CenterObject.h"

void CenterObject::Init(MeshModel* model, const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	meshModel_ = model;
	emitter_ = std::make_unique<Emitter>(100, 1, 1.0f, 2.5f, 0.5f,
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	object_ = std::make_unique<MeshGPUParticle>(timer, matView, matProjection, emitter_.get(), meshModel_);
}

void CenterObject::Update(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	float rotPower = 1.0f;

	if (rot_.y <= 180.0f) {
		rot_.y += rotPower;
	}
	else {
		rot_.y = 0;
	}

	DirectX::XMFLOAT3 rot = MyMathConvert::ChangeVector3toXMfloat3(rot_);

	emitter_->SetRotation(rot);

	object_->Update(timer, matView, matProjection, emitter_.get());
}

void CenterObject::Draw(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	object_->Draw(timer, matView, matProjection, emitter_.get());
}

void CenterObject::SetObject(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color)
{
	DirectX::XMFLOAT3 nowPos = MyMathConvert::ChangeVector3toXMfloat3(pos);
	DirectX::XMFLOAT3 nowRot = MyMathConvert::ChangeVector3toXMfloat3(rotation);
	DirectX::XMFLOAT3 nowScale = MyMathConvert::ChangeVector3toXMfloat3(scale);
	DirectX::XMFLOAT4 nowColor = MyMathConvert::ChangeXMFLOAT4(color);
	float nowParticleSize = 1.5f;

	emitter_->SetPosition(nowPos);
	emitter_->SetScaling(nowScale);
	emitter_->SetRotation(nowRot);
	emitter_->SetStartColor(nowColor);
	emitter_->SetParticleSize(nowParticleSize);
}
