#pragma once
#include "MyMath.h"
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>

class Light {
public:
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		DirectX::XMVECTOR lightv;     // ライトの方向ベクトル
		DirectX::XMFLOAT3 lightColor; // ライトの色
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
	void SetLightDir(const DirectX::XMVECTOR& lightDir);

	// 色セット
	void SetLightRGB(const DirectX::XMFLOAT3& lightRGB);

private:
	// 定数バッファ生成
	void CreateConstBuffer();

	// 定数バッファ転送
	void TransferConstBuffer();

private:
	// 光線方向(単位ベクトル)
	DirectX::XMVECTOR lightDir_ = {1, 0, 0, 0};
	// 色
	DirectX::XMFLOAT3 lightRGB_ = {1, 1, 1};
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// ダーティーフラグ
	bool isDirty = false;
};
