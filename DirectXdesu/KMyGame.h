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
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �I��
	/// </summary>
	void Final();
	/// <summary>
	/// �I�����N�G�X�g
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

