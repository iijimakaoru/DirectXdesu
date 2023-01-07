#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class BossActState
{
protected:
	bool finished = false;
public:
	virtual ~BossActState() {}

	bool IsFinished() {
		return finished;
	}

	virtual void Update() = 0;
	virtual void Draw() = 0;
};

