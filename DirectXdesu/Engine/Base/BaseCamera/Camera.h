#pragma once
#include "ViewProjection.h"
#include "KWinApp.h"
#include <memory>

class Camera
{
public:
	std::unique_ptr<ViewProjection> viewProjection = nullptr;

public:
	virtual ~Camera() {}

	virtual void Init() = 0;

	virtual void Update() = 0;
};