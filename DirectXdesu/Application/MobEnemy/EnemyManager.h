#pragma once
#include <list>
#include <memory>
#include "MobEnemy.h"

class Player;

class EnemyManager
{
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <returns></returns>
	static EnemyManager* Create(const std::string fileName, Player* player_, KModel* model_, KGPlin* pipeline_);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Player* player_, KModel* model_, KGPlin* pipeline_);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 出ている全ての敵を消す
	void AllEnemyDelete();

	// ゲッター
	const std::list<std::unique_ptr<MobEnemy>>& GetMobEnemys() const;

private:
	// 条件にあった敵の削除
	void DeleteEnemy();

	// 敵発生データの読み込み
	void LoadEnemyPopData(const std::string fileName);

	// 敵発生コマンドの更新
	void UpdateEnemyPopCommands();

private:
	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	bool isStand = false;
	int32_t waitTimer = 0;

	// パイプライン
	KGPlin* pipeline;

	// プレイヤー情報格納
	Player* player = nullptr;

private:
	// 雑魚
	std::list<std::unique_ptr<MobEnemy>> mobEnemys;
	KModel* model1;
};

