#include "ObjectSetter.h"

void ObjectSetter::Init(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	centerModel_ = std::make_unique<MeshModel>("suzanne1");

	centerObject_ = std::make_unique<CenterObject>();
	centerObject_->Init(centerModel_.get(), timer, matView, matProjection);
}

void ObjectSetter::Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	centerObject_->Update(timer, matView, matProjection);
}

void ObjectSetter::Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	centerObject_->Draw(timer, matView, matProjection);
}

void ObjectSetter::SetCenterObject(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
	const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	centerObject_->SetObject(pos, rotation, scale, color, timer, matView, matProjection);
}
