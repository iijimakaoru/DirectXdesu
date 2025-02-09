#include "CalibrationCallback.h"

bool CalibrationCallback::Capture()
{
    return callBack;
}

void CalibrationCallback::SetCapture(bool flag)
{
	callBack = flag;
}


