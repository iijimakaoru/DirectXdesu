#pragma once
#include "KGPlin.h"
#include "KShader.h"
#include "KDirectXCommon.h"
#include "KObject3d.h"

#pragma warning(push)
#pragma warning(disable: 4820)
#include <forward_list>
#pragma warning(pop)

class Particles
{
public:// サブクラス
	// 頂点データ構造体
	struct VertexPos
	{
		KMyMath::Vector3 pos; // xyz座標
		float scale;
	};

	// 定数バッファ用データ構造体
	struct PConstBufferData
	{
		KMyMath::Matrix4 mat;	// ３Ｄ変換行列
		KMyMath::Matrix4 matBillboard;
	};

	struct Particle
	{
		KMyMath::Vector3 pos = {};

		KMyMath::Vector3 vel = {};

		KMyMath::Vector3 accel = {};

		int frame = 0;

		int num_frame = 0;

		float scale = 1.0f;

		float s_scale = 1.0f;

		float e_scale = 0.0f;
	};

private: // 定数
	static const int vertexCount = 1024;		// 頂点数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Particles* Create(TextureData& textureData_);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;

	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;

	// パイプライン
	static std::unique_ptr<KGPlin> pipeline;

	// シェーダー
	static KShader shader;

	// ビルボード行列
	static DirectX::XMMATRIX matBillboard;
	// Y軸回りビルボード行列
	static DirectX::XMMATRIX matBillboardY;

private:// 静的メンバ関数
	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

public: // メンバ関数
	bool Initialize(TextureData textureData_);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(ViewProjection* viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクル追加
	/// </summary>
	/// <param name="life"></param>
	/// <param name="pos"></param>
	/// <param name="vel"></param>
	/// <param name="accel"></param>
	/// <param name="start_scale"></param>
	/// <param name="end_scale"></param>
	void Add(int life, KMyMath::Vector3 pos, KMyMath::Vector3 vel, KMyMath::Vector3 accel, float start_scale, float end_scale);

	/// <summary>
	/// モデル作成
	/// </summary>
	void CreateModel();

private:
	// 頂点データ配列
	VertexPos vertices[vertexCount];

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;

private: // メンバ変数
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff; // 定数バッファ

	TextureData textureData;

	std::forward_list<Particle> particles;
};

class ParticleManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 爆発
	/// </summary>
	void CallExp(const KMyMath::Vector3& pos);

	/// <summary>
	/// 小爆発
	/// </summary>
	void CallSmallExp(const KMyMath::Vector3& pos);

private:
	// インスタンス
	static ParticleManager* parthicleManager;

	// テクスチャ
	TextureData textureData1;
	TextureData textureData2;

	// パーティクル
	std::unique_ptr<Particles> particles1;
	std::unique_ptr<Particles> particles2;

public:
	// インスタンス
	static ParticleManager* GetInstance();

	// 解放
	void Delete();

private:
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = default;
	const ParticleManager& operator=(const ParticleManager&) = delete;
};

class ObjParticle
{
public:
	/// <summary>
	/// 作成
	/// </summary>
	/// <returns></returns>
	static ObjParticle* Create(const KMyMath::Vector3& pos_, 
		KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& velocity, TextureData& tex);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos_"></param>
	/// <param name="model_"></param>
	/// <param name="pipeline_"></param>
	void Init(const KMyMath::Vector3& pos_, 
		KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& velocity, TextureData& tex);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 死んだかゲッター
	/// </summary>
	/// <returns></returns>
	const bool GetIsDead() const
	{
		return isDead;
	}

private:
	// オブジェクト
	std::unique_ptr<KObject3d> object3d = nullptr;

	// 速度
	KMyMath::Vector3 velocity = { 0,0,0 };

	// 生存時間
	uint32_t lifeTimer = 0;
	const uint32_t lifeTime = 60;

	float easeTimer = 0;
	const float easeTime = 20;

	// 死んだかどうか
	bool isDead = false;

	// テクスチャ
	TextureData texture;
};

class ObjParticleManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 爆発演出
	/// </summary>
	/// <param name="pos_"></param>
	void SetExp(const KMyMath::Vector3& pos_);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos_"></param>
	void SetSmallExp(const KMyMath::Vector3& pos_);

public:
	// インスタンス
	static ObjParticleManager* GetInstance();

	// 解放
	void Delete();

private:
	// インスタンス
	static ObjParticleManager* objParticleManager;

	// パイプライン
	KShader shader;
	std::unique_ptr<KGPlin> pipeline = nullptr;

	// モデル
	std::unique_ptr<KModel> model = nullptr;

	// パーティクルズ
	std::list<std::unique_ptr<ObjParticle>> objParticles;

	// テクスチャ
	TextureData textureData1;

private:
	ObjParticleManager() = default;
	~ObjParticleManager() = default;
	ObjParticleManager(const ObjParticleManager&) = default;
	const ObjParticleManager& operator=(const ObjParticleManager&) = delete;
};