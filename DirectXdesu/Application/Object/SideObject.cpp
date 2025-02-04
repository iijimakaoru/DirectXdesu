#include "SideObject.h"

void SideObject::Init(MeshModel* model, const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
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

void SideObject::Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	MoveUpdate();

	RotUpdate();

	object_->Update(timer, matView, matProjection, emitter_.get());
}

void SideObject::Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	object_->Draw(timer, matView, matProjection, emitter_.get());
}

void SideObject::SetObject(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color)
{
	DirectX::XMFLOAT3 nowPos = MyMathConvert::ChangeVector3toXMfloat3(pos);
	DirectX::XMFLOAT3 nowRot = MyMathConvert::ChangeVector3toXMfloat3(rotation);
	DirectX::XMFLOAT3 nowScale = MyMathConvert::ChangeVector3toXMfloat3(scale);
	DirectX::XMFLOAT4 nowColor = MyMathConvert::ChangeXMFLOAT4(color);
	float nowParticleSize = 0.5f;

	emitter_->SetPosition(nowPos);
	emitter_->SetScaling(nowScale);
	emitter_->SetRotation(nowRot);
	emitter_->SetStartColor(nowColor);
	emitter_->SetParticleSize(nowParticleSize);

	rotPower_ = { MyMathUtility::GetRandF(-1.0f,1.0f),MyMathUtility::GetRandF(-1.0f,1.0f) ,MyMathUtility::GetRandF(-1.0f,1.0f) };
	move_ = pos;
	movePower_ = 1.0f;
	isDead = false;
}

bool SideObject::GetIsDead()
{
	return isDead;
}

void SideObject::MoveUpdate()
{
	move_.z -= movePower_;

	DirectX::XMFLOAT3 move = MyMathConvert::ChangeVector3toXMfloat3(move_);

	emitter_->SetPosition(move);

	if (emitter_->GetPosition().z < -130.0f)
	{
		isDead = true;
	}
}

void SideObject::RotUpdate()
{
	rot_ += rotPower_;

	DirectX::XMFLOAT3 rot = MyMathConvert::ChangeVector3toXMfloat3(rot_);

	emitter_->SetRotation(rot);
}
