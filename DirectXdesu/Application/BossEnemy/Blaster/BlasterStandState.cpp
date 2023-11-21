#include "BlasterStandState.h"
#include "Blaster.h"

BlasterStandState::BlasterStandState()
{
	actTime = 300;

	unitPos = { MyMathUtility::GetRand(-1.0f,1.0f),0.0f,0.0f };
}

void BlasterStandState::Update()
{
	Blaster* blaster = Blaster::nowBlaster;
	if (actTimer < actTime)
	{
		actTimer++;
		blaster->SetUnitsPos({  unitPos.x, 6.0f, 0.0f }, 0);
		blaster->SetUnitsPos({  0.0f,-6.0f, 0.0f }, 1);
	}
	else
	{
		isFinish = true;
	}
}
