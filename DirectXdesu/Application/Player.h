#pragma once
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"
#include "KInput.h"
#include "PlayerBullet.h"

#include "Reticle3D.h"
#include "Reticle2D.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model_"></param>
	/// <returns></returns>
	static Player* Create(KModel* model_,KGPlin* objPipeline_, const float playerHP, KGPlin* spritePipeline_);

public:
	// 初期化
	void Init(KModel* model_, KGPlin* objPipeline_, const float playerHP, KGPlin* spritePipeline_);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera_"></param>
	void Update(ViewProjection* viewPro);

	// 描画
	// オブジェクト
	void ObjDraw();

	// ゲームスプライト
	void SpriteDraw();

	// UI
	void UIDraw();

	// 衝突時に呼び出し
	void OnCollision();

	/// <summary>
	/// 親子セッター
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransfom* parent);

	// ポジションゲッター
	const KMyMath::Vector3& GetPosition() const;

	// ワールドポジションゲッター
	const KMyMath::Vector3 GetWorldPos() const;

	// 回転ゲッター
	const KMyMath::Vector3 GetRot() const;

	// 死んでるか
	const bool GetIsDead() const;

	// 死亡演出が終わったか
	const bool GetIsFallEffectEnd() const;

public:
	// ゲッター
	static const KMyMath::Vector2& GetRotLimit()
	{ 
		return rotLimit;
	}

	static const float GetMoveSpeed() 
	{ 
		return moveSpeed; 
	}

	static const KMyMath::Vector2& GetPosLimitMax() 
	{ 
		return posLimitMax; 
	}

	static const KMyMath::Vector2& GetPosLimitMin() 
	{ 
		return posLimitMin; 
	}

	const bool GetIsInvisible() const
	{
		return isInvisible;
	}

	void SetPos(const KMyMath::Vector3 pos_);

private:
	// 移動
	void Move();

	// 回転
	void Rot();

	// 攻撃
	void Attack();

	// 死亡演出
	void DeadEffect();

	// HP演出
	void HPEffect();

	// ダメージエフェクト
	void DamageEffect();

	// デバッグ用
	void Debug();

private:
#pragma region 大元の変数
	// Input
	KInput* input = nullptr;

	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	KModel* model = nullptr;

	// パイプライン
	KGPlin* objPipeline;
	KGPlin* spritePipeline;
#pragma endregion

#pragma region 回転用変数
	// 回転戻すイージング用
	KMyMath::Vector2 oldRot;
	float rotEaseTimer = 0;
	bool isRotEase = false;

	// Z軸
	bool isRotZRight = false;
	float swayZ = 0.0f;
#pragma endregion

#pragma region レティクル
	// 2Dに変換するための3Dレティクル
	std::unique_ptr<Reticle3D> reticle3d = nullptr;
	// 最終的なレティクル
	std::unique_ptr<Reticle2D> reticle2d = nullptr;
#pragma endregion

#pragma region 制限
	// 動くスピード
	static const float moveSpeed;

	// 角度上限
	static const KMyMath::Vector2 rotLimit;

	// 移動上限
	static const KMyMath::Vector2 posLimitMax;
	static const KMyMath::Vector2 posLimitMin;
#pragma endregion

#pragma region HP関連
	// 最大体力
	float maxHP;

	// 現体力
	float HP;
	float oldHP;
	float startHpEase;

	uint32_t oldHpTimer = 0;
	const uint32_t oldHpTime = 60;

	bool hpEase = false;
	float hpEaseTimer = 0;
	const float hpEaseTime = 15;

	// 死亡しているか
	bool isDead;

	// HP
	std::unique_ptr<Sprite> HPUI = nullptr;
	TextureData hpTex;
	std::unique_ptr<Sprite> HPrectUI = nullptr;

	// HPバー
	std::unique_ptr<Sprite> HPBarUI = nullptr;
	TextureData hpbarTex;

	// 無敵時間
	uint32_t invisibleTimer = 0;
	const uint32_t invisibleTime = 30;
	bool isInvisible = false;

	KMyMath::Vector4 hpColor = { 0,0,0,1.0f };
#pragma endregion

#pragma region 被弾演出
	bool isDamageEffect = false;
	std::unique_ptr<Sprite> damage = nullptr;
	TextureData damageTex;
	float dAlpha = 0;
#pragma endregion

#pragma region 墜落用変数
	// 演出時間
	float fallEffectTime = 90;
	float fallEffectTimer = 0;

	// 爆発クールタイム
	float expTime = 10;
	float expTimer = 0;

	bool isFallEffectEnd = false;
#pragma endregion

};