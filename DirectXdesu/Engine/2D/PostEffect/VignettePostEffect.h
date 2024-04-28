#pragma once
#include "PostEffect.h"
class VignettePostEffect : public BasePostEffect {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画情報
	/// </summary>
	void DrawCommand() override;

	// シーン描画前処理
	void PreDrawScene() override;

	// シーン描画後処理
	void PostDrawScene() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	struct ConstBuff {
		KMyMath::Vector3 colorRGB;
		float vignetteNum;
	};

private:
	void CreateConstBuff();

private:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;
};
