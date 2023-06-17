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
	// 初期化
	void Init() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了
	void Final() override;

private:
	std::unique_ptr<BaseScene> scene = nullptr;

	ImguiManager imguiMane;
};

