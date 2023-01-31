#include "DebugCamera.h"
#include "KInput.h"

DebugCamera::DebugCamera()
{
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::Init(Vector3 eye, Vector3 target, Vector3 up)
{
	this->eye = eye;
	this->target = target;
	this->up = up;

	MatUpdate();

	frontVec = target - eye;
	disEyeTarget = frontVec.Length();
}

void DebugCamera::Update()
{
	moveCursor = KInput::GetMousePos() - KInput::GetOldMousePos();
	float cursorDisPrev = moveCursor.Length();
	moveCursor.normalize();

	if (input->GetMouseClick(KInput::Left))
	{
		moveCursor /= 1000;
		moveCursor *= cursorDisPrev;
		if (up.y < 0) {
			moveCursor.x = -moveCursor.x;
		}
		cursorSpeed += moveCursor;
	}
	disEyeTarget += -KInput::GetMouseWheel() * (disEyeTarget * 0.001f);
	if (disEyeTarget < 10)
	{
		disEyeTarget = 10;
	}
	target += rightVec * (KInput::GetInstance()->IsPush(DIK_A) - KInput::GetInstance()->IsPush(DIK_D));
	target += downVec * (KInput::GetInstance()->IsPush(DIK_S) - KInput::GetInstance()->IsPush(DIK_W));
	target += -frontVec * (KInput::GetInstance()->IsPush(DIK_Z) - KInput::GetInstance()->IsPush(DIK_X));

	frontVec = target - eye;
	frontVec.Normalize();
	rightVec = Vector3(0, 1, 0).Cross(frontVec);
	downVec = rightVec.Cross(frontVec);

	if (rotAngle.x >= PI) rotAngle.x -= PI;
	if (rotAngle.x < 0) rotAngle.x += PI;
	if (rotAngle.y >= PI) rotAngle.y -= PI;
	if (rotAngle.y < 0) rotAngle.y += PI;

	Vector2 angle = rotAngle;
	angle += cursorSpeed;

	rightVec.Normalize();
	downVec.Normalize();

	up.y = cosf(angle.y);
	eye.x = target.x - disEyeTarget * cosf(angle.y) * sinf(angle.x);
	eye.y = target.y + disEyeTarget * sinf(angle.y);
	eye.z = target.z - disEyeTarget * cosf(angle.y) * cosf(angle.x);
	MatUpdate();
}
