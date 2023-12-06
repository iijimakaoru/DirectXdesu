#pragma once
#include "EnemyBullet.h"
#include "PlayerBullet.h"

/**
 * @file BulletManager.h
 * @brief 弾のマネージャー
 * @author 飯島 薫
 */

class BulletManager {
public:
	void Init(KGPlin* pipeline_);

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
	    const float bulletSpeed);

	/// <summary>
	/// 敵の弾発射
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="vec_"></param>
	/// <param name="rot_"></param>
	/// <param name="bulletSpeed"></param>
	void EnemyBulletShot(
	    const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_,
	    const float bulletSpeed);

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

private:
	// 弾の削除
	void DeleteBullet();

private:
	// プレイヤーの弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
	// KModel* playersBulletModel = nullptr;
	std::unique_ptr<KModel> playersBulletModel = nullptr;

	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	std::unique_ptr<KModel> enemysBulletModel = nullptr;

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
