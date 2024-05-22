#pragma once
#include "KGPlin.h"
#include "KModel.h"
#include "Vector3.h"
#include "Camera.h"
#include "Transform.h"
#include "Light.h"

/**
 * @file KObject3D.h
 * @brief オブジェクト3D
 * @author 飯島 薫
 */

class KObject3d {
public:
	// デストラクタ
	~KObject3d();

public:
	static std::unique_ptr<Light> light_;

	struct ConstBufferDataB0 {
		KMyMath::Vector4 color;
		KMyMath::Matrix4 viewPro;
		KMyMath::Matrix4 world;
		KMyMath::Vector3 cameraPos;
	};

public:
	static void StaticInit();

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="pipeline_"></param>
	/// <returns></returns>
	static KObject3d* Create(KModel* model_, KGPlin* pipeline_);

	static void SetLight(Light* light);

private:
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

	// 定数バッファマテリアル
	void CreateCBMaterial();

	// 頂点、インデックス生成
	void CreateVertexIndex();

	// 定数バッファトランスフォーム
	void CreateCBTransform();

public:
	/// <summary>
	/// トランスフォーム更新
	/// </summary>
	void TransUpdate();

	/// <summary>
	/// 行列更新
	/// </summary>
	/// <param name="viewProjection"></param>
	void MatUpdate(KMyMath::Matrix4& matView, ViewProjection* viewPro, const KMyMath::Vector3& cameraPos);

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
	KGPlin* pipeline;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;

	// 定数バッファ(マテリアル)
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

private:
	HRESULT result;

	KModel* model = nullptr;

	// 3Dオブジェクトの配列
	Transform transform;

	KMyMath::Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
};
