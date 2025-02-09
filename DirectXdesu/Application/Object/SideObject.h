#pragma once
#include <memory>
#include "Emitter.h"
#include "MeshGPUParticle.h"

class SideObject
{
public:
	void Init(MeshModel* model, const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	void Update(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);
	void Draw(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection);

	void SetObject(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color);

	bool GetIsDead();

	void SetVibration();

private:
	void MoveUpdate();
	void RotUpdate();
	void ScaleUpdate();

private:
	MeshModel* meshModel_ = nullptr;
	std::unique_ptr<Emitter> emitter_;
	std::unique_ptr<MeshGPUParticle> object_;

	KMyMath::Vector3 rot_;
	KMyMath::Vector3 rotPower_;

	KMyMath::Vector3 startScale_;
	KMyMath::Vector3 endScale_;

	KMyMath::Vector3 move_;
	float movePower_;

	bool isDead = false;

	bool isVibration = false;

	float scaleEaseTimer_;
	float scaleEaseLimit_;
};

