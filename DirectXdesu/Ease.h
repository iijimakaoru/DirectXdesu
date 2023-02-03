#pragma 
#include <math.h>
#include "Vector3.h"

float Lerp(const float start, const float end, const float time);
float EaseIn(const float start, const float end, const float time, const float power);
Vector3 EaseInVec3(const Vector3 start, const Vector3 end, const float time, const float power);
float EaseOut(const float start, const float end, const float time, const float power);

const Vector3 lerp3D(const Vector3& start, const Vector3& end, const float t);