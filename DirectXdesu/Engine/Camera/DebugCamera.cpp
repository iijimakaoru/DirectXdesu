#include "DebugCamera.h"
#include "KInput.h"

void DebugCamera::Init()
{
	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();
}

void DebugCamera::Update()
{
	viewProjection->Update();
}