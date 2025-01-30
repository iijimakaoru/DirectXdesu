#pragma once
#include <memory>
#include "Emitter.h"
#include "MeshGPUParticle.h"

class CenterObject
{
public:
	void Init(MeshModel* model, const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	void Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	void Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

	void SetObject(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color,
		const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

private:
	MeshModel* meshModel_ = nullptr;
	std::unique_ptr<Emitter> emitter_;
	std::unique_ptr<MeshGPUParticle> object_;
};

