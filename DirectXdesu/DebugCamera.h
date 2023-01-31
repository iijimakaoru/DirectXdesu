#pragma once
#include "Camera.h"
#include "KInput.h"

class DebugCamera : public Camera
{
private:
	KInput* input = KInput::GetInstance();

	Vector2 moveCursor;
	Vector2 cursorSpeed;

	Vector2 rotAngle;
	Vector2 prevRotAngle;
	Vector2 endRotAngle;

public:
	DebugCamera();
	~DebugCamera() override;

	void Init(Vector3 eye, Vector3 target, Vector3 up) override;
	void Update() override;
};

