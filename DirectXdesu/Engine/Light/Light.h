#pragma once
#include "MyMath.h"
#include <d3d12.h>
#include <wrl.h>

class Light {
public:
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		KMyMath::Vector4 lightv;     // ライトの方向ベクトル
		KMyMath::Vector3 lightColor; // ライトの色
	};

public:
	// 生成
	static Light* Create();

	// 初期化
	void Init();

	// 更新
	void Update();

	// 描画
	void Draw(UINT rootParamIndex);

	// 方向セット
	void SetLightDir(const KMyMath::Vector4& lightDir);

	// 色セット
	void SetLightRGB(const KMyMath::Vector3& lightRGB);

private:
	// 定数バッファ生成
	void CreateConstBuffer();

	// 定数バッファ転送
	void TransferConstBuffer();

private:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// 光線方向(単位ベクトル)
	KMyMath::Vector4 lightDir_ = {1, 0, 0, 0};
	// 色
	KMyMath::Vector3 lightRGB_ = {1, 1, 1};
	// ダーティーフラグ
	bool isDirty = false;
};
