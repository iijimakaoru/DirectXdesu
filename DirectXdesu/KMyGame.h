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
#include "ParticleManager.h"

#include "FbxLoader.h"

class KMyGame
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 終了
	/// </summary>
	void Final();
	/// <summary>
	/// 終了リクエスト
	/// </summary>
	bool IsEndRequest();

private:
	FbxLoader* fbxLoader = nullptr;

	KWinApp* win = nullptr;

	KDirectXCommon* dx = nullptr;

	KInput* input = nullptr;

	std::unique_ptr<GameScence> gameScene = nullptr;

	ImguiManager imguiMane;
};

