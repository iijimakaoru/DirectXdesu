#pragma once
#include <stdio.h>
#include <d3dcompiler.h>
#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#ifdef _DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")
#include "Sound.h"
#include <d3dx12.h>
#include "GameScence.h"
#include "ImguiManager.h"

#include "FbxLoader.h"

#include "Framework.h"

#include "ImguiManager.h"

#include "BaseScene.h"

class KMyGame : public Framework
{
public:
	// ‰Šú‰»
	void Init() override;
	// XV
	void Update() override;
	// •`‰æ
	void Draw() override;
	// I—¹
	void Final() override;

private:
	std::unique_ptr<BaseScene> scene = nullptr;

	ImguiManager imguiMane;
};

