#include "BossStart.h"
#include "RailCamera.h"
#include "Player.h"

BossStart::BossStart() { cameraPhase = WaitBlackOut; }

void BossStart::Update() {
	switch (cameraPhase) {
	case BossStart::WaitBlackOut:
		break;
	case BossStart::Descent:
		break;
	case BossStart::Approach:
		break;
	case BossStart::BreakTime1:
		break;
	case BossStart::OpenUnit:
		break;
	case BossStart::BreakTime2:
		break;
	case BossStart::Distant:
		break;
	case BossStart::BlackOut:
		break;
	case BossStart::SetBoss:
		break;
	case BossStart::Exit:
		break;
	default:
		break;
	}
}

void BossStart::WaitBlackOutPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	phaseTime = 30;

	if (phaseTimer < phaseTime) {
		phaseTimer++;
	} else {
		// すべての弾削除
		bulletManager_->AllBulletDelete();
		// プレイヤーとカメラの親子関係解消
		player->SetParent(nullptr);
		// 現在位置まで連れてくる
		player->SetPos({0.0f, 0.0f, player->GetWorldPos().z});
		// 回転角度初期化
		player->SetRot({0.0f, 0.0f, 0.0f});
		phaseTimer = 0;
		cameraPhase = Descent;
	}
}
