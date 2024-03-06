#include "EnemyManager.h"
#include "AppearEnemy.h"
#include "CanonEnemy.h"
#include "FlyEnemy.h"
#include "Player.h"
#include "RailCamera.h"
#include "ScoreManager.h"
#include "ModelManager.h"
#include "PipelineManager.h"

EnemyManager* EnemyManager::Create(
    const std::string fileName_, Player* player_) {
	// インスタンス
	EnemyManager* instance = new EnemyManager();
	if (instance == nullptr) {
		return nullptr;
	}

	instance->Init(player_);
	instance->LoadEnemyPopData(fileName_);

	return instance;
}

void EnemyManager::Init(Player* player_) {
	// プレイヤー情報格納
	player = player_;
}

void EnemyManager::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	// 敵出現
	UpdateEnemyPopCommands();

	// 雑魚敵の更新
	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys) {
		const KMyMath::Vector3 pos = player->GetWorldPos();
		mobEnemy->Update(viewPro, cameraPos);
	}

	// 条件削除
	DeleteEnemy();
}

void EnemyManager::Draw() {
	// 雑魚敵描画
	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys) {
		mobEnemy->Draw();
	}
}

void EnemyManager::AllEnemyDelete() {
	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys) {
		mobEnemy->SelfDestruction();
	}
}

const std::list<std::unique_ptr<MobEnemy>>& EnemyManager::GetMobEnemys() const { return mobEnemys; }

void EnemyManager::DeleteEnemy() {
	// 敵消去
	mobEnemys.remove_if([](std::unique_ptr<MobEnemy>& MobEnemy_) {
		if (MobEnemy_->GetIsDead() || MobEnemy_->GetIsDelete()) {
			return true;
		}

		return false;
	});
}

void EnemyManager::LoadEnemyPopData(const std::string fileName_) {
	//"Resources/csv/enemyPop.csv"

	// ファイルを開く
	std::ifstream file;
	file.open(fileName_);
	assert(file.is_open());

	// ファイルの内容を文字ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void EnemyManager::UpdateEnemyPopCommands() {
	// 待機処理
	if (isStand) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機終了
			isStand = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字数をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// コメント
		if (word.find("//") == 0) {
			// 行を飛ばす
			continue;
		}

		// POP
		if (word.find("POP") == 0) {
			// 敵の種類
			getline(line_stream, word, ',');
			size_t enemyType = static_cast<size_t>(std::atof(word.c_str()));

			// x座標
			getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// y座標
			getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// z座標
			getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			/// 敵を発生させる
			// 生成
			std::unique_ptr<MobEnemy> newMEnemy;

			// 該当するタイプの敵生成
			if (enemyType == MobEnemy::EnemysType::Fly) {
				// x座標
				getline(line_stream, word, ',');
				float _x = static_cast<float>(std::atof(word.c_str()));

				// y座標
				getline(line_stream, word, ',');
				float _y = static_cast<float>(std::atof(word.c_str()));

				// 生成
				newMEnemy.reset(FlyEnemy::Create(
				    ModelManager::GetInstance()->GetModels("MobEnemy1"), // モデルセット
				    PipelineManager::GetInstance()->GetObjPipeline(), // パイプラインセット
				    {x, y, z},             // 生成場所
				    {_x, _y},              // 最終地点
				    RailCamera::GetSpeed() // 移動スピード
				    ));

				// 自機狙い弾用にプレイヤーセット
				newMEnemy->SetPlayer(player);

				// 登録
				mobEnemys.push_back(std::move(newMEnemy));
			} else if (enemyType == MobEnemy::EnemysType::Canon) {
				// 生成
				newMEnemy.reset(CanonEnemy::Create(
				    ModelManager::GetInstance()->GetModels("MobEnemy1"), // モデルセット
				    PipelineManager::GetInstance()->GetObjPipeline(), // パイプラインセット
				    {x, y, z} // 出現位置
				    ));

				// 自機狙い弾用にプレイヤーセット
				newMEnemy->SetPlayer(player);

				// 登録
				mobEnemys.push_back(std::move(newMEnemy));
			} else if (enemyType == MobEnemy::EnemysType::Appear) {
				// 生成
				newMEnemy.reset(AppearEnemy::Create(
				    ModelManager::GetInstance()->GetModels("MobEnemy1"), // モデルセット
				    PipelineManager::GetInstance()->GetObjPipeline(), // パイプラインセット
				    {x, y, z} // 出現位置
				    ));

				// 自機狙い弾用にプレイヤーセット
				newMEnemy->SetPlayer(player);

				// 登録
				mobEnemys.push_back(std::move(newMEnemy));
			}

			ScoreManager::GetInstance()->AddDestoryCountMax();
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isStand = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}
