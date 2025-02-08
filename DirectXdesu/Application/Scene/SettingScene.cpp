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
		SceneManager::GetInstance()->ChangeScene("GAME");
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
			if (ImGui::Button("InterCalibrateCamera0",{360,240}))
			{
				InterinsicCalibrating = true;
				interCalibratingAsync = std::async(std::launch::async, [=]() {
					return captureManager->GetYOLOPoseEstimation()->InterinsCalibrateStart(0);
					});
				interSicNum = 0;
			}
			else if (ImGui::Button("InterCalibrateCamera1",{360,240}))
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

			if (ImGui::Button("ExtrinsCalibrateCameraEnd", { 360,240 }))
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
			if (ImGui::Button("ExtrinsCalibrateCamera0Start",{ 360,240 }))
			{
				ExtrinsiCalibrating = true;
				extrinsiCalibratingAsync = std::async(std::launch::async, [=]() {
					return captureManager->GetYOLOPoseEstimation()->ExtrinsCalibrateStart(0);
					});
				extrinSicNum = 0;
			}
			else if (ImGui::Button("ExtrinsCalibrateCamera1Start",{ 360,240 }))
			{
				ExtrinsiCalibrating = true;
				extrinsiCalibratingAsync = std::async(std::launch::async, [=]() {
					return captureManager->GetYOLOPoseEstimation()->ExtrinsCalibrateStart(1);
					});
				extrinSicNum = 1;
			}
		}
		if (ImGui::TreeNode("CalibrateDataCamera:0"))
		{
			IntrinsicParameterCalibrator::Parameter intParam = captureManager->GetYOLOPoseEstimation()
																->GetInterinsParameter(0);

			ImGui::Text("\n\n\n");
			ImGui::Text("CameraMatrix");
			ImGui::Text("%d,%d,%d", intParam.cameraMatrix.Get(0, 0),
				intParam.cameraMatrix.Get(1, 0), intParam.cameraMatrix.Get(2, 0));
			ImGui::Text("%d,%d,%d", intParam.cameraMatrix.Get(0, 1),
				intParam.cameraMatrix.Get(1, 1), intParam.cameraMatrix.Get(2, 1));
			ImGui::Text("%d,%d,%d", intParam.cameraMatrix.Get(0, 2),
				intParam.cameraMatrix.Get(1, 2), intParam.cameraMatrix.Get(2, 2));

			ImGui::Text("\n\n\n");
			ImGui::Text("DistCoefee");
			ImGui::Text("%d,%d,%d,%d,%d", intParam.distortionCoefficients.GetX(),
				intParam.distortionCoefficients.GetY(), intParam.distortionCoefficients.GetZ(),
				intParam.distortionCoefficients.GetW(), intParam.distortionCoefficients.GetV());

			
			ExtrinsiParameterCalibrator::Parameter extParam = captureManager->GetYOLOPoseEstimation()
				->GetExtrinsiParameter(0);

			ImGui::Text("\n\n\n");
			ImGui::Text("CameraRotateMatrix");
			ImGui::Text("%d,%d,%d", extParam.rotationMatrix.Get(0, 0),
				extParam.rotationMatrix.Get(1, 0), extParam.rotationMatrix.Get(2, 0));
			ImGui::Text("%d,%d,%d", extParam.rotationMatrix.Get(0, 1),
				extParam.rotationMatrix.Get(1, 1), extParam.rotationMatrix.Get(2, 1));
			ImGui::Text("%d,%d,%d", extParam.rotationMatrix.Get(0, 2),
				extParam.rotationMatrix.Get(1, 2), extParam.rotationMatrix.Get(2, 2));
			
			ImGui::Text("\n\n\n");
			ImGui::Text("CameraTranslateVector");
			ImGui::Text("%d,%d,%d", extParam.translationVector.GetX(), extParam.translationVector.GetY(),
										extParam.translationVector.GetZ());

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("CalibrateDataCamera:1"))
		{
			IntrinsicParameterCalibrator::Parameter intParam = captureManager->GetYOLOPoseEstimation()
				->GetInterinsParameter(1);

			ImGui::Text("\n\n\n");
			ImGui::Text("CameraMatrix");
			ImGui::Text("%d,%d,%d", intParam.cameraMatrix.Get(0, 0),
				intParam.cameraMatrix.Get(1, 0), intParam.cameraMatrix.Get(2, 0));
			ImGui::Text("%d,%d,%d", intParam.cameraMatrix.Get(0, 1),
				intParam.cameraMatrix.Get(1, 1), intParam.cameraMatrix.Get(2, 1));
			ImGui::Text("%d,%d,%d", intParam.cameraMatrix.Get(0, 2),
				intParam.cameraMatrix.Get(1, 2), intParam.cameraMatrix.Get(2, 2));

			ImGui::Text("\n\n\n");
			ImGui::Text("DistCoefee");
			ImGui::Text("%d,%d,%d,%d,%d", intParam.distortionCoefficients.GetX(),
				intParam.distortionCoefficients.GetY(), intParam.distortionCoefficients.GetZ(),
				intParam.distortionCoefficients.GetW(), intParam.distortionCoefficients.GetV());


			ExtrinsiParameterCalibrator::Parameter extParam = captureManager->GetYOLOPoseEstimation()
				->GetExtrinsiParameter(1);

			ImGui::Text("\n\n\n");
			ImGui::Text("CameraRotateMatrix");
			ImGui::Text("%d,%d,%d", extParam.rotationMatrix.Get(0, 0),
				extParam.rotationMatrix.Get(1, 0), extParam.rotationMatrix.Get(2, 0));
			ImGui::Text("%d,%d,%d", extParam.rotationMatrix.Get(0, 1),
				extParam.rotationMatrix.Get(1, 1), extParam.rotationMatrix.Get(2, 1));
			ImGui::Text("%d,%d,%d", extParam.rotationMatrix.Get(0, 2),
				extParam.rotationMatrix.Get(1, 2), extParam.rotationMatrix.Get(2, 2));

			ImGui::Text("\n\n\n");
			ImGui::Text("CameraTranslateVector");
			ImGui::Text("%d,%d,%d", extParam.translationVector.GetX(), extParam.translationVector.GetY(),
				extParam.translationVector.GetZ());

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();
}
