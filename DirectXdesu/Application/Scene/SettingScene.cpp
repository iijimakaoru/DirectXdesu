#include "SettingScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

#include "Ease.h"

#include "ModelManager.h"
#include "PipelineManager.h"



#include "PostEffectManager.h"

const int gNumberFrameResources = 3;

SettingScene::~SettingScene() { Final(); }

void SettingScene::LoadResources() {
	// 天球モデル
	skyDomeModel = ModelManager::GetInstance()->GetModels("S_SkyDorm");
}

void SettingScene::Init() {

	BaseScene::Init();

	// カメラ読み込み
	camera = std::make_unique<TitleCamera>();
	camera->Init();

	sceneManager = SceneManager::GetInstance();

	audioManager = AudioManager::GetInstance();

	imgui = ImguiManager::GetInstance();

	captureManager = MCBM::CaptureManager::GetInstance();

	camera->StartRound();

	
}

void SettingScene::Update() {

	if (input->IsTrigger(DIK_1)) {

	}
	ImguiUpdate();
	camera->Update();
}

void SettingScene::ObjDraw() {



}

void SettingScene::SpriteDraw() {

}

void SettingScene::Final() {

}

void SettingScene::ImguiUpdate()
{
	ImGui::Begin("CalibrateInfo");
	if (ImGui::TreeNode("Calibrate"))
	{
		if (InterinsicCalibrating)
		{
			std::string text = "InterCalibrationCamera:" + std::to_string(interSicNum);
			ImGui::Text(text.c_str());
			auto status = interCalibratingAsync.wait_for(std::chrono::milliseconds(0));
			bool isfinite = status == std::future_status::ready;
			if (isfinite)
			{
				InterinsicCalibrating = false;
				captureManager->GetYOLOPoseEstimation()->InterinsCalibrateSave("Resources\\CalibrateData");
			}
		}
		else
		{
			if (ImGui::Button("InterCalibrateCamera0"))
			{
				InterinsicCalibrating = true;
				interCalibratingAsync = std::async(std::launch::async, [=]() {
					return captureManager->GetYOLOPoseEstimation()->InterinsCalibrateStart(0);
					});
				interSicNum = 0;
			}
			else if (ImGui::Button("InterCalibrateCamera1"))
			{
				InterinsicCalibrating = true;
				interCalibratingAsync = std::async(std::launch::async, [=]() {
					return captureManager->GetYOLOPoseEstimation()->InterinsCalibrateStart(1);
					});
				interSicNum = 1;
			}

		}
		if (ExtrinsiCalibrating)
		{
			std::string text = "ExtrinsCalibrationCamera:" + std::to_string(extrinSicNum);
			ImGui::Text(text.c_str());

			if (ImGui::Button("ExtrinsCalibrateCameraEnd"))
			{
				callBack.SetCapture(true);
			}
			auto status = extrinsiCalibratingAsync.wait_for(std::chrono::milliseconds(0));
			bool isfinite = status == std::future_status::ready;
			if (isfinite)
			{
				callBack.SetCapture(false);
				captureManager->GetYOLOPoseEstimation()->ExtrinsCalibrateSave("Resources\\CalibrateData");
			}
		}
		else
		{
			if (ImGui::Button("ExtrinsCalibrateCamera0Start"))
			{
				ExtrinsiCalibrating = true;
				extrinsiCalibratingAsync = std::async(std::launch::async, [=]() {
					return captureManager->GetYOLOPoseEstimation()->ExtrinsCalibrateStart(0);
					});
				extrinSicNum = 0;
			}
			else if (ImGui::Button("ExtrinsCalibrateCamera1Start"))
			{
				ExtrinsiCalibrating = true;
				extrinsiCalibratingAsync = std::async(std::launch::async, [=]() {
					return captureManager->GetYOLOPoseEstimation()->ExtrinsCalibrateStart(1);
					});
				extrinSicNum = 1;
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();
}
