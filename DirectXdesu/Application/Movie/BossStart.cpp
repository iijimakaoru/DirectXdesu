#include "BossStart.h"
#include "Blaster.h"
#include "Player.h"
#include "RailCamera.h"

void (BossStart::*BossStart::phase[])() = {
    &BossStart::WaitBlackOutPhase, 
	&BossStart::DescentPhase,  
	&BossStart::ApproachPhase,
    &BossStart::BreakTimePhase1,   
	&BossStart::OpenUnitPhase, 
	&BossStart::BreakTimePhase2,
    &BossStart::DistantPhase,      
	&BossStart::BlackOutPhase, 
	&BossStart::SetBossPhase,
    &BossStart::ExitPhase
};

BossStart::BossStart() {
	SetIsFinish(false);
	cameraPhase = WaitBlackOut;
	sceneChange_ = SceneChange::GetInstance();
	audioManager_ = AudioManager::GetInstance();
	bulletManager_ = BulletManager::GetInstance();
	input_ = KInput::GetInstance();
}

void BossStart::Update() {
	// 関数テーブル更新
	(this->*phase[cameraPhase])();

	// スキップ
	if (cameraPhase < BlackOut) {
		if (input_->GetPadButtonDown(XINPUT_GAMEPAD_START)) {
			cameraPhase = BlackOut;
			phaseTimer_ = 0;
		}
	}
}

void BossStart::WaitBlackOutPhase() {
	Player* player = Player::nowPlayer;

	phaseTime_ = 30;

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;
	} else {
		// すべての弾削除
		bulletManager_->AllBulletDelete();
		// プレイヤーとカメラの親子関係解消
		player->SetParent(nullptr);
		// 現在位置まで連れてくる
		player->SetPos({0.0f, 0.0f, player->GetWorldPos().z});
		// 回転角度初期化
		player->SetRot({0.0f, 0.0f, 0.0f});
		phaseTimer_ = 0;
		cameraPhase = Descent;
	}
}

void BossStart::DescentPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Blaster* blaster = Blaster::nowBlaster;

	phaseTime_ = 90.0f;

	MovieBarInInit();

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;

		// ボス降下
		const float startBPosY = 40.0f;
		const float endBPosY = 20.0f;
		blaster->SetPos(
		    {blaster->GetWorldPos().x,
		     MyEase::OutCubicFloat(startBPosY, endBPosY, phaseTimer_ / phaseTime_),
		     blaster->GetWorldPos().z});

		// カメラの最終位置
		KMyMath::Vector3 cameraMove = {0.0f, 0.0f, -30.0f};

		// カメラの場所
		const KMyMath::Vector3 cameraPos = {
		    blaster->GetWorldPos().x + cameraMove.x, endBPosY + cameraMove.y,
		    blaster->GetWorldPos().z + cameraMove.z};

		camera->SetCameraPos(cameraPos);

		const KMyMath::Vector3 rot = {0.0f, 0.0f, 0.0f};
		camera->SetCameraRot(rot);
	} else {
		phaseTimer_ = 0;
		cameraPhase = Approach;
	}
}

void BossStart::ApproachPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Blaster* blaster = Blaster::nowBlaster;

	phaseTime_ = 30.0f;

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;

		// 自機とカメラの距離
		KMyMath::Vector3 cameraMove = {
		    0.0f, 0.0f, MyEase::OutCubicFloat(-30.0f, -20.0f, phaseTimer_ / phaseTime_)};

		// カメラの場所
		const KMyMath::Vector3 cameraPos = {
		    blaster->GetWorldPos().x + cameraMove.x, blaster->GetWorldPos().y + cameraMove.y,
		    blaster->GetWorldPos().z + cameraMove.z};

		camera->SetCameraPos(cameraPos);

		const KMyMath::Vector3 rot = {0.0f, 0.0f, 0.0f};
		camera->SetCameraRot(rot);
	} else {
		phaseTimer_ = 0;
		cameraPhase = BreakTime1;
	}
}

void BossStart::OpenUnitPhase() {
	Blaster* blaster = Blaster::nowBlaster;

	phaseTime_ = 10.0f;

	if (phaseTimer_ < phaseTime_) {
		if (phaseTimer_ == 0) {
			audioManager_->SEPlay_wav("bossAwakenSE.wav");
		}

		phaseTimer_++;

		const float startNum = 3.0f;
		const float endNum = 4.0f;

		std::array<KMyMath::Vector3, 8> unitMove;
		unitMove[0] = MyEase::Lerp3D(
		    {startNum, startNum, startNum}, {endNum, endNum, endNum}, phaseTimer_ / phaseTime_);
		unitMove[1] = MyEase::Lerp3D(
		    {-startNum, startNum, startNum}, {-endNum, endNum, endNum}, phaseTimer_ / phaseTime_);
		unitMove[2] = MyEase::Lerp3D(
		    {startNum, startNum, -startNum}, {endNum, endNum, -endNum}, phaseTimer_ / phaseTime_);
		unitMove[3] = MyEase::Lerp3D(
		    {-startNum, startNum, -startNum}, {-endNum, endNum, -endNum}, phaseTimer_ / phaseTime_);
		unitMove[4] = MyEase::Lerp3D(
		    {startNum, -startNum, startNum}, {endNum, -endNum, endNum}, phaseTimer_ / phaseTime_);
		unitMove[5] = MyEase::Lerp3D(
		    {-startNum, -startNum, startNum}, {-endNum, -endNum, endNum}, phaseTimer_ / phaseTime_);
		unitMove[6] = MyEase::Lerp3D(
		    {startNum, -startNum, -startNum}, {endNum, -endNum, -endNum}, phaseTimer_ / phaseTime_);
		unitMove[7] = MyEase::Lerp3D(
		    {-startNum, -startNum, -startNum}, {-endNum, -endNum, -endNum},
		    phaseTimer_ / phaseTime_);

		for (size_t i = 0; i < 8; i++) {
			blaster->SetUnitsPos(unitMove[i], i);
		}
	} else {
		phaseTimer_ = 0;
		cameraPhase = BreakTime2;
	}
}

void BossStart::DistantPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Blaster* blaster = Blaster::nowBlaster;

	phaseTime_ = 180.0f;

	if (phaseTimer_ < phaseTime_) {
		if (phaseTimer_ == 0) {
			blaster->SetFarstAct();
		}

		phaseTimer_++;

		const float startNum = 4.0f;
		const float endNum = 6.0f;

		std::array<KMyMath::Vector3, 8> unitMove;
		unitMove[0] = MyEase::Lerp3D(
		    {startNum, startNum, startNum}, {endNum, endNum, endNum}, phaseTimer_ / phaseTime_);
		unitMove[1] = MyEase::Lerp3D(
		    {-startNum, startNum, startNum}, {-endNum, endNum, endNum}, phaseTimer_ / phaseTime_);
		unitMove[2] = MyEase::Lerp3D(
		    {startNum, startNum, -startNum}, {endNum, endNum, -endNum}, phaseTimer_ / phaseTime_);
		unitMove[3] = MyEase::Lerp3D(
		    {-startNum, startNum, -startNum}, {-endNum, endNum, -endNum}, phaseTimer_ / phaseTime_);
		unitMove[4] = MyEase::Lerp3D(
		    {startNum, -startNum, startNum}, {endNum, -endNum, endNum}, phaseTimer_ / phaseTime_);
		unitMove[5] = MyEase::Lerp3D(
		    {-startNum, -startNum, startNum}, {-endNum, -endNum, endNum}, phaseTimer_ / phaseTime_);
		unitMove[6] = MyEase::Lerp3D(
		    {startNum, -startNum, -startNum}, {endNum, -endNum, -endNum}, phaseTimer_ / phaseTime_);
		unitMove[7] = MyEase::Lerp3D(
		    {-startNum, -startNum, -startNum}, {-endNum, -endNum, -endNum},
		    phaseTimer_ / phaseTime_);

		for (size_t i = 0; i < 8; i++) {
			blaster->SetUnitsPos(unitMove[i], i);
		}

		// カメラの動き
		KMyMath::Vector3 cameraMove = MyEase::InOutCubicVec3(
		    {0.0f, 0.0f, -20.0f}, {10.0f, -20.0f, -120.0f}, phaseTimer_ / phaseTime_);

		// カメラの場所
		const KMyMath::Vector3 cameraPos = blaster->GetWorldPos() + cameraMove;
		camera->SetCameraPos(cameraPos);

		const KMyMath::Vector3 rot = {
		    0.0f, MyEase::InOutCubicFloat(0.0f, -15.0f, phaseTimer_ / phaseTime_),
		    MyEase::InOutCubicFloat(0.0f, 15.0f, phaseTimer_ / phaseTime_)};
		camera->SetCameraRot(rot);
	} else {
		phaseTimer_ = 0;
		cameraPhase = BlackOut;
	}
}

void BossStart::BlackOutPhase() {
	phaseTime_ = 30;

	if (phaseTimer_ == 0) {
		audioManager_->BGMPlay_wav("bossBGM.wav", 0.15f);
		sceneChange_->SceneChangeStart();
	}

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;
	} else {
		phaseTimer_ = 0;
		cameraPhase = SetBoss;
	}
}

void BossStart::SetBossPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;
	Blaster* blaster = Blaster::nowBlaster;

	blaster->SetFarstAct();
	MovieBarOutInit();
	// ボス配置
	blaster->SetPos({blaster->GetWorldPos().x, 20.0f, blaster->GetWorldPos().z});
	blaster->SetRot({0.0f, 0.0f, 0.0f});
	// カメラ配置
	camera->SetCameraPos({0.0f, 0.0f, 500.0f});
	camera->SetCameraRot({0.0f, 0.0f, 0.0f});
	// プレイヤーとカメラの親子関係解消
	player->SetParent(&camera->GetTransform());
	// 現在位置まで連れてくる
	player->SetPos({0.0f, 0.0f, 50.0f});
	phaseTimer_ = 0;
	cameraPhase = Exit;
}

void BossStart::ExitPhase() { SetIsFinish(true); }

void BossStart::BreakTimePhase1() {
	phaseTime_ = 30.0f;

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;
	} else {
		phaseTimer_ = 0;
		cameraPhase = OpenUnit;
	}
}

void BossStart::BreakTimePhase2() {
	phaseTime_ = 30.0f;

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;
	} else {
		phaseTimer_ = 0;
		cameraPhase = Distant;
	}
}
