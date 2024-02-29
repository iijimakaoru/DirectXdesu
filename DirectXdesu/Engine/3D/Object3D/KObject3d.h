#pragma once
#include "KGPlin.h"
#include "KModel.h"
#include "Vector3.h"
#include "ViewProjection.h"

/**
 * @file KObject3D.h
 * @brief オブジェクト3D
 * @author 飯島 薫
 */

struct ConstBufferDataB1 {
	KMyMath::Vector3 ambient;
	float pad1;
	KMyMath::Vector3 diffuse;
	float pad2;
	KMyMath::Vector3 specular;
	float alpha;
};

struct ConstBufferDataB0 {
	KMyMath::Vector4 color;
	KMyMath::Matrix4 mat;
};

struct WorldTransfom {
	// アフィン変換
	KMyMath::Vector3 scale = {1, 1, 1};
	KMyMath::Vector3 rot = {0, 0, 0};
	KMyMath::Vector3 pos = {0, 0, 0};

	// ワールド変換行列
	KMyMath::Matrix4 matWorld = {};

	// 親オブジェクトへのポインタ
	const WorldTransfom* parent = nullptr;
};

class KObject3d {
public:
	static void StaticInit();

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="pipeline_"></param>
	/// <returns></returns>
	static KObject3d* Create(KModel* model_, KGPlin* pipeline_);

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
	void MatUpdate(ViewProjection* viewProjection_);

	/// <summary>
	/// 上の複合
	/// </summary>
	/// <param name="viewProjection"></param>
	void Update(ViewProjection* viewProjection_);

	/// <summary>
	/// デフォルトテクスチャ描画
	/// </summary>
	void Draw();

	/// <summary>
	/// テクスチャ切り替え描画
	/// </summary>
	/// <param name="texture"></param>
	void Draw(TextureData& texData_);

	~KObject3d() { Finalize(); }

	void Finalize();

	// ゲッター
	const KMyMath::Vector3& GetPos() const;
	const KMyMath::Vector3& GetRot() const;
	const KMyMath::Vector3& GetScale() const;
	const KMyMath::Matrix4& GetMatWorld() const;
	const WorldTransfom& GetTransform() const;

	// セッター
	void SetPos(const KMyMath::Vector3& pos_);
	void SetRot(const KMyMath::Vector3& rot_);
	void SetScale(const KMyMath::Vector3& scale_);
	void SetParent(const WorldTransfom* parent_);
	void AddSetPos(const KMyMath::Vector3& pos_);
	void AddSetRot(const KMyMath::Vector3& rot_);
	void AddSetScale(const KMyMath::Vector3& scale_);
	void SetColor(const KMyMath::Vector4& color_);

private:
	KGPlin* pipeline;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;

	// 定数バッファ(マテリアル)
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

private:
	HRESULT result;

	KModel* model = nullptr;

	// 3Dオブジェクトの配列
	WorldTransfom transform;

	KMyMath::Vector4 color = {1, 1, 1, 1};
};
