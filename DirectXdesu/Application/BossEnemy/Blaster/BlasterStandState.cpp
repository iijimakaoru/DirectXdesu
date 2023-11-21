#include "BlasterStandState.h"
#include "Blaster.h"
#include "Ease.h"

BlasterStandState::BlasterStandState()
{
	actStartTime = 30.0f;

	actTime = 300;

	unitPos = { MyMathUtility::GetRand(-1.0f,1.0f),0.0f,0.0f };
}

void BlasterStandState::Update()
{
	Blaster* blaster = Blaster::nowBlaster;
	if (actStartTimer < actStartTime)
	{
		actStartTimer++;
		blaster->SetUnitsPos(MyEase::Lerp3D(blaster->GetUnitsPos(0), { 0.0f, 9.0f, 0.0f }, actStartTimer / actStartTime), 0);
		blaster->SetUnitsPos(MyEase::Lerp3D(blaster->GetUnitsPos(1), { 0.0f,-9.0f, 0.0f }, actStartTimer / actStartTime), 1);
	}
	else
	{
		if (actTimer < actTime)
		{
			actTimer++;
		}
		else
		{
			isFinish = true;
		}
	}
}
