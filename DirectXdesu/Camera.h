#pragma once
#include "ViewProjection.h"
#include "KWinApp.h"
#include <memory>

class Camera
{
public:
	ViewProjection viewProjection;

public:
	virtual ~Camera() {}

	virtual void Update() = 0;
};