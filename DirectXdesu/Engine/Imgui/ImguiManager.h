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
	// 初期化
	void Init();

	// 最終処理
	void Finalize();

	// 更新
	void Update();

	// 処理の始め
	void Begin();

	// 描画
	void Draw();

	// 処理の終わり
	void End();

private:
	KWinApp* win = KWinApp::GetInstance();
	KDirectXCommon* dx = KDirectXCommon::GetInstance();
};

