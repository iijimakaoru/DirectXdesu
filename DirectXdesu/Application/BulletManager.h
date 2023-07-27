#pragma once
#include "PlayerBullet.h"
#include "EnemyBullet.h"

class BulletManager
{
public:
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
	void PlayerBulletShot(KModel* model, KGPlin* pipeline_,
		const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed);

	void EnemyBulletShot();

	// インスタンスゲッター
	static BulletManager* GetInstance();

	// 解放
	void Delete();

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

	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;

	// インスタンス
	static BulletManager* bulletManager;

private:
	BulletManager() = default;
	~BulletManager() = default;
	BulletManager(const BulletManager&) = delete;
	const BulletManager& operator=(const BulletManager&) = delete;
};

