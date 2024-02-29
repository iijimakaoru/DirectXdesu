#pragma once
#include "Bom.h"
#include "EnemyBullet.h"
#include "PlayerBullet.h"
#include "UnitLazer.h"

/**
 * @file BulletManager.h
 * @brief 弾のマネージャー
 * @author 飯島 薫
 */

class BulletManager {
public:
	void Init();

	/// <summary>
	/// 弾の更新
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro);

	/// <summary>
	/// 弾の描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーの弾発射
	/// </summary>
	/// <param name="model"></param>
	/// <param name="pipeline_"></param>
	/// <param name="pos"></param>
	/// <param name="vec_"></param>
	/// <param name="rot_"></param>
	/// <param name="bulletSpeed"></param>
	void PlayerBulletShot(
	    const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
	    const float bulletSpeed, const float BulletPower_);

	/// <summary>
	/// 敵の弾発射
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="vec_"></param>
	/// <param name="rot_"></param>
	/// <param name="bulletSpeed"></param>
	void EnemyBulletShot(
	    const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
	    const float bulletSpeed, const float BulletPower_);

	/// <summary>
	/// ボム発射
	/// </summary>
	/// <param name="pos_"></param>
	/// <param name="vec_"></param>
	/// <param name="rot_"></param>
	/// <param name="bulletSpeed_"></param>
	void BomShot(
	    const KMyMath::Vector3& pos_, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
	    const float bulletSpeed_);

	void UnitLazerSet(const KMyMath::Vector3& pos_, const KMyMath::Vector3& rot_, uint32_t& i);

	void UnitLazerCall();

	void UnitLazerDelete();

	// インスタンスゲッター
	static BulletManager* GetInstance();

	// 存命の弾を全部削除
	void AllBulletDelete();

	// 解放
	void Delete();

public:
	// 自弾のリストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetPlayerBullets() const;

	// 敵弾のリストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetEnemyBullets() const;

	// ボムのリスト所得
	const std::list<std::unique_ptr<Bom>>& GetBoms() const;

	const KMyMath::Vector3 GetLazersPos(size_t i) const;

	void LazerOpen(const float& timer,size_t i);

private:
	// 弾の削除
	void DeleteBullet();

private:
	// プレイヤーの弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;

	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;

	// ボム
	std::list<std::unique_ptr<Bom>> boms;

	// ユニットレーザー
	std::array<std::unique_ptr<UnitLazer>, 16> unitLazers;

	// パイプライン
	KGPlin* pipeline = nullptr;

	// インスタンス
	static BulletManager* bulletManager;

private:
	BulletManager() = default;
	~BulletManager() = default;
	BulletManager(const BulletManager&) = delete;
	const BulletManager& operator=(const BulletManager&) = delete;
};
