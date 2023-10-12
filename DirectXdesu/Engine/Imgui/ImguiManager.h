#pragma once
#include <imgui.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "KWinApp.h"
#include "KDirectXCommon.h"

/**
 * @file ImguiManager.h
 * @brief Imguiのマネージャー
 * @author 飯島 薫
 */

class ImguiManager
{
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	void Init();
	void Finalize();
	void Update();
	void Begin();
	void Draw();
	void End();

private:
	KWinApp* win = KWinApp::GetInstance();
	KDirectXCommon* dx = KDirectXCommon::GetInstance();
};

