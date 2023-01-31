#include "Camera.h"

void Camera::MatUpdate()
{
	matView = MyMath::GetInstance()->LockAt(eye, target, up);
}
