#pragma once
#include "Camera.h"

class TitleCamera : public Camera
{
public:
	TitleCamera()
	{
		Init();
	}

	void Init();

	void Update()override;
};

