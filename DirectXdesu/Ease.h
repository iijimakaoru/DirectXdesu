#pragma 
#include <math.h>
#include "Vector3.h"

float Lerp(const float start, const float end, const float time);
float EaseIn(const float start, const float end, const float time, const float power);
KMyMath::Vector3 EaseInVec3(const KMyMath::Vector3 start, const KMyMath::Vector3 end, const float time, const float power);
float EaseOut(const float start, const float end, const float time, const float power);

