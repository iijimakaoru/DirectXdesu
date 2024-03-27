#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "KInput.h"
#include "KModel.h"
#include "KObject3d.h"
#include "KShader.h"
#include "PlayerBullet.h"

#include "Reticle2D.h"
#include "Reticle3D.h"

#include "AudioManager.h"
#include "Light.h"

#include "Shake.h"

#include "Collider.h"

/**
 * @file Player.h
 * @brief 自機
 * @author 飯島 薫
 */

/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider
{
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model_"></param>
	/// <returns></returns>
	static Player* Create(const float playerHP);

public:
	// 初期化
	void Init(const float playerHP);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera_"></param>
	void Update(
	    ViewProjection* viewPro, const KMyMath::Vector3& cameraPos, bool isStart_,
	    bool isBossMovie_, bool isClearMovie_);

	// 描画
	// オブジェクト
	void ObjDraw();

	// ゲームスプライト
	void SpriteDraw();

	// UI
	void UIDraw();

	// スタートエフェクト終わり
	void EndStart();

	// 定位置へ
	void StandStartPos();

	/// <summary>
	/// 親子セッター
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const Transform* parent);

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
	// 衝突時に呼び出し
	void OnCollision() override;

	KMyMath::Vector3 GetWorldPosition() override;

public:
	// ゲッター
	static const KMyMath::Vector2& GetRotLimit();
	static const float GetMoveSpeed();
	static const KMyMath::Vector2& GetPosLimitMax();
	static const KMyMath::Vector2& GetPosLimitMin();
	const bool GetIsInvisible() const;

	// セッター
	void SetPos(const KMyMath::Vector3& pos_);
	void SetRot(const KMyMath::Vector3& rot_);
	void SetScale(const KMyMath::Vector3& scale_);

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

	// 弾クールタイム
	void SudCoolTime();

	// デバッグ用
	void Debug();

public:
	static Player* nowPlayer;

	static bool isStartEase;

private:
#pragma region 大元の変数
	// Input
	KInput* input = nullptr;

	// オブジェクト
	std::unique_ptr<KObject3d> object3d;
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
	float maxHP = 0;

	// 現体力
	float HP = 0;
	float oldHP = 0;
	float startHpEase = 0;

	uint32_t oldHpTimer = 0;
	const uint32_t oldHpTime = 60;

	bool hpEase = false;
	float hpEaseTimer = 0;
	const float hpEaseTime = 15;

	// スタート時のHPイージング
	KMyMath::Vector2 HPPos = {-500.0f, 0.0f};
	KMyMath::Vector2 HPSize = {1.0f, 1.0f};

	// 死亡しているか
	bool isDead;

	// HP
	std::unique_ptr<Sprite> HPUI = nullptr;
	TextureData hpTex;
	KMyMath::Vector2 HPUIPos = {14.0f, 6.0f};
	std::unique_ptr<Sprite> HPrectUI = nullptr;

	// HPバー
	std::unique_ptr<Sprite> HPBarUI = nullptr;
	TextureData hpbarTex;
	KMyMath::Vector2 HPBarUIPos = {10.0f, 10.0f};

	bool isCrisis = false;

	// 無敵時間
	uint32_t invisibleTimer = 0;
	const uint32_t invisibleTime = 30;
	bool isInvisible = false;

	// 弾間隔
	uint32_t coolTimeSet = 10;
	uint32_t coolTimer = 0;

	// HP色
	KMyMath::Vector4 hpColor = {0, 0, 0, 1.0f};
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
	float expTime = 15;
	float expTimer = 0;

	bool isFallEffectEnd = false;
#pragma endregion

#pragma region ステージ始めのイージング
	float startEaseTimer = 0;
	float startEaseTime = 0;
#pragma endregion

	bool isStartMovie = false;
	bool isBossMovie = false;
	bool isClearMovie = false;

#pragma region 操作説明
	std::unique_ptr<Sprite> operation;
	TextureData operationTex;
	KMyMath::Vector2 operationPos;
#pragma endregion

#pragma region ボム関連
	// ボム個数
	size_t bomsCount = 3;

	// ボムクールタイム
	float bomsCoolTime = 120.0f;
	float bomsCoolTimer = bomsCoolTime;

	// ボムのアイコン
	std::unique_ptr<Sprite> bomIcon = nullptr;
	TextureData bomIconTex;
	KMyMath::Vector2 bomIconPos = {0.0f, 0.0f};
	float bomIconAlpha = 1.0f;
	bool isBom = false;

	// ボムのクールタイム用
	std::unique_ptr<Sprite> bomCoolIcon = nullptr;
#pragma endregion

	bool muteki = false;

	AudioManager* audioManager = nullptr;

	Shake2D hpShake;
};