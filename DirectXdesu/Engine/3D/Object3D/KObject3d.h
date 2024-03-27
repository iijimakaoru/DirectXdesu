#pragma once
#include "KGPlin.h"
#include "KModel.h"
#include "Vector3.h"
#include "Camera.h"
#include "Transform.h"
#include "Light.h"
#include "CollisionInfo.h"

/**
 * @file KObject3D.h
 * @brief オブジェクト3D
 * @author 飯島 薫
 */

class BaseCollider;

class KObject3d {
public:
	// コンストラクタ
	KObject3d() = default;

	// デストラクタ
	virtual ~KObject3d();

public:
	static std::unique_ptr<Light> light_;

	struct ConstBufferDataB0 {
		KMyMath::Vector4 color;
		KMyMath::Matrix4 viewPro;
		KMyMath::Matrix4 world;
		KMyMath::Vector3 cameraPos;
	};

public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="pipeline_"></param>
	/// <returns></returns>
	static KObject3d* Create(KModel* model_, KGPlin* pipeline_);

	static void SetLight(Light* light);

public:
	// 初期化
	void Initialize();

	/// <summary>
	/// モデル読み込み(セッター)
	/// </summary>
	/// <param name="model"></param>
	void LoadModel(KModel* model_);

	/// <summary>
	/// パイプラインセッター
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

public:
	/// <summary>
	/// トランスフォーム更新
	/// </summary>
	void TransUpdate();

	/// <summary>
	/// 行列更新
	/// </summary>
	/// <param name="viewProjection"></param>
	void MatUpdate(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos);

	/// <summary>
	/// 上の複合
	/// </summary>
	/// <param name="viewProjection"></param>
	void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos);

	/// <summary>
	/// デフォルトテクスチャ描画
	/// </summary>
	void Draw();

	/// <summary>
	/// テクスチャ切り替え描画
	/// </summary>
	/// <param name="texture"></param>
	void Draw(TextureData& texData_);

	void Finalize();

	// ゲッター
	Transform& GetTransform();

	// セッター
	void SetParent(const Transform* parent_);
	void SetColor(const KMyMath::Vector4& color_);
	void SetRGB(const KMyMath::Vector3& rgb);
	void SetAlpha(const float& a);

private:
	HRESULT result_;

	KGPlin* pipeline;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;

	// 定数バッファ(マテリアル)
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

protected:
	KModel* model = nullptr;

	// 3Dオブジェクトの配列
	Transform transform;

	KMyMath::Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};

	BaseCollider* baseCollider_ = nullptr;
};
