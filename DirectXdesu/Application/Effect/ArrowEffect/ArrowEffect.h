#pragma once
#include <memory>
#include "Emitter.h"
#include "MeshGPUParticle.h"

class ArrowEffect
{
public:
	static ArrowEffect* Create(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale,KMyMath::Vector4& color, 
		MeshModel* model, const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

	void Init(KMyMath::Vector3& pos,KMyMath::Vector3& rotation,KMyMath::Vector3& scale, KMyMath::Vector4& color,
		const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

	void Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

	void Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

	bool GetIsDead();

private:
	void SetModel(MeshModel* model);
	void SetLifeLimit(float& lifeLimit);
	
private:
	MeshModel* meshModel_ = nullptr;
	std::unique_ptr<Emitter> emitter_;
	std::unique_ptr<MeshGPUParticle> effect_;

	KMyMath::Vector3 startScale_;
	KMyMath::Vector3 endScale_;

	float lifeLimit_ = 0;
	float lifeTimer_ = 0;
	bool isDead = false;
};

