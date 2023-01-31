#pragma once
#include "ViewProjection.h"
#include "KWinApp.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "MyMath.h"

class Camera
{
protected:
	Matrix matView;
	Vector3 eye;
	Vector3 target;
	Vector3 up;
	Vector3 frontVec;
	Vector3 rightVec;
	Vector3 downVec;

	float disEyeTarget = 0.0f;

	Matrix matProjection = MyMath::GetInstance()->PerspectiveFov(MyMath::GetInstance()->ConvertToRad(48.0f), 0.1f, 1000.0f);

public:
	virtual ~Camera() = default;
	virtual void Init(Vector3 eye, Vector3 target, Vector3 up) = 0;
	virtual void Update() = 0;
	void MatUpdate();
};

