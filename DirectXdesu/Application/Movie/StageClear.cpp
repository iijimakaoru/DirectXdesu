#include "StageClear.h"
#include "Player.h"
#include "RailCamera.h"

void (StageClear::*StageClear::phase[])() = {
	&StageClear::InsertPhase,
	&StageClear::RoundPhase,
	&StageClear::FlyPhase,
	&StageClear::BlackOutPhase,
	&StageClear::GoResultPhase
};

StageClear::StageClear() {
	audioManager_ = AudioManager::GetInstance();
	gameManager_ = GameManager::GetInstance();
	sceneChange_ = SceneChange::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	bulletManager_ = BulletManager::GetInstance();
	input_ = KInput::GetInstance();
	BaseMovie::Init();
	phaseTimer_ = 0;
	cameraPhase = CameraPhase::Insert;
}

void StageClear::Update() {
	// 関数テーブル更新
	(this->*phase[cameraPhase])();

	// スキップ
	if (cameraPhase < BlackOut) {
		if (input_->GetPadButtonDown(XINPUT_GAMEPAD_START)) {
			cameraPhase = BlackOut;
			audioManager_->SoundStopWave("mokuhyo.wav");
			phaseTimer_ = 0;
		}
	}
}

void StageClear::InsertPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	phaseTime_ = 30.0f;

	if (phaseTimer_ < phaseTime_) {
		if (phaseTimer_ == 0) {
			audioManager_->SoundStopWave("bossBGM.wav");
			audioManager_->SEPlay_wav("mokuhyo.wav");
		}

		phaseTimer_++;

		MovieBarIn(phaseTimer_ / phaseTime_);

		player->SetPos(MyEase::InCubicVec3(
		    player->GetWorldPos(), {0.0f, 0.0f, player->GetWorldPos().z},
		    phaseTimer_ / phaseTime_));
		player->SetRot(
		    MyEase::InCubicVec3(player->GetRot(), {0.0f, 0.0f, 0.0f}, phaseTimer_ / phaseTime_));

		const KMyMath::Vector3 dhistans = {0.0f, 0.0f, -40.0f};

		const KMyMath::Vector3 playerPos = {0.0f, 0.0f, player->GetWorldPos().z};

		const KMyMath::Vector3 cameraPos = playerPos + dhistans;

		camera->SetCameraPos(
		    MyEase::InOutCubicVec3(camera->GetCameraPos(), cameraPos, phaseTimer_ / phaseTime_));

		camera->SetCameraRot({0.0f, 0.0f, 0.0f});
	} else {
		phaseTimer_ = 0;
		cameraPhase = CameraPhase::Round;
	}
}

void StageClear::RoundPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	phaseTime_ = 180.0f;

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;

		// 角度を変更
		float rotAngle = MyEase::InOutCubicFloat(0.0f, -60.0f, phaseTimer_ / phaseTime_);

		const float radian = DirectX::XMConvertToRadians(rotAngle);
		const float distance = -30;

		const KMyMath::Vector3 cameraPos = {
		    (distance * sinf(radian)), 0.0f, (distance * cosf(radian))};
		camera->SetCameraPos(player->GetWorldPos() + cameraPos);

		camera->SetCameraRot({0.0f, rotAngle, 0.0f});
	} else {
		phaseTimer_ = 0;
		cameraPhase = CameraPhase::Fly;
	}
}

void StageClear::FlyPhase() {
	RailCamera* camera = RailCamera::nowRailCamera;
	Player* player = Player::nowPlayer;

	if (phaseTimer_ == 0) {
		start = {0.0f, 0.0f, player->GetWorldPos().z};
		p1 = {-20.0f, 0.0f, player->GetWorldPos().z + 75.0f};
		p2 = {-50.0f, 75.0f, player->GetWorldPos().z + 100.0f};
		end = {-100.0f, 100.0f, player->GetWorldPos().z + 125.0f};
	}

	phaseTime_ = 210.0f;

	if (phaseTimer_ < phaseTime_) {
		phaseTimer_++;

		// ポイント１の制御点
		KMyMath::Vector3 point1_1 =
		    MyEase::OutCubicVec3(start, p1, phaseTimer_ / phaseTime_);
		KMyMath::Vector3 point1_2 = MyEase::OutCubicVec3(p1, end, phaseTimer_ / phaseTime_);
		KMyMath::Vector3 point1 =
		    MyEase::OutCubicVec3(point1_1, point1_2, phaseTimer_ / phaseTime_);

		// ポイント２の制御点
		KMyMath::Vector3 point2_1 = MyEase::OutCubicVec3(start, p2, phaseTimer_ / phaseTime_);
		KMyMath::Vector3 point2_2 = MyEase::OutCubicVec3(p2, end, phaseTimer_ / phaseTime_);
		KMyMath::Vector3 point2 =
		    MyEase::OutCubicVec3(point2_1, point2_2, phaseTimer_ / phaseTime_);

		player->SetPos(MyEase::OutCubicVec3(point1, point2, phaseTimer_ / phaseTime_));
		player->SetScale(MyEase::InCubicVec3({2.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, phaseTimer_ / phaseTime_));
		player->SetRot(MyEase::OutCubicVec3(
		    {0.0f, 0.0f, 0.0f}, {-45.0f, -45.0f, 45.0f}, phaseTimer_ / phaseTime_));

		camera->SetCameraRot(
		    {MyEase::OutCubicFloat(0.0f, -15.0f, phaseTimer_ / phaseTime_),
		     camera->GetCameraRot().y, camera->GetCameraRot().z});
	} else {
		phaseTimer_ = 0;
		cameraPhase = CameraPhase::BlackOut;
	}
}

void StageClear::BlackOutPhase() {
	sceneChange_->SceneChangeStart();
	gameManager_->SetIsStartMovie(false);
	cameraPhase = CameraPhase::GoResult;
}

void StageClear::GoResultPhase() {
	if (sceneChange_->GetIsChange()) {
		sceneManager_->ChangeScene("CLEAR");
		bulletManager_->AllBulletDelete();
	}
}
