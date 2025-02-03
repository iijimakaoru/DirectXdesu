#pragma once
#include <memory>
#include <unordered_map>

#include <Animation/Bone.h>
#include <Animation/Capture.h>
#include <Animation/AnimationManager.h>

namespace MCBM
{
	class Skelton
	{
	private:
		std::vector<std::unique_ptr<Bone>> bones_;
		
		Bone* rootBone_;

		Capture* capture;

	public:
		void SetCapturePtr(Capture* cap);

		void AddBone(std::unique_ptr<Bone> bone);

		Bone* GetBone(std::string name);

		MQuaternion GetBoneRotation(std::string name);

		//キャプチャーデータの読み込み
		void UpdateCaptureData();

		//モデルの初期姿勢の設定（プレイヤーにモデルの素のポーズをまねてもらう）
		void CaptureBasePoseInitialize();

		void SetRootBone(Bone* bone);

		void CaptureBoneAccept();

		void Finalize();
		/// <summary>
		/// モデルの回転の計算
		/// </summary>
		/// <param name="rootBoneName">キャプチャさせたい根本のKey(例:YOLO_POSE_INDEX::SHOULDER_L)</param>
		/// <param name="boneCount">どのくらい先端のボーンまでやるか。腕なら2で良い</param>
		void CaptureBoneUpdate(YOLO_POSE_INDEX rootBoneName, uint32_t boneCount = 2);

		void readAnimNodeHeirarchy(float animationTime, Bone* pNode, bool& animationPositionRock, Animation* currentAnimationPtr = nullptr);//実際に階層構造読み込んでAnimationの計算をする関数

		static const NodeAnim* findNodeAnim(const Animation* pAnimation, const std::string& NodeName);//どのNodeのキーフレームを見るのか
		//キーフレームの補完-------------
		static void calcInterpolatedPosition(MVector3& Out, float AnimationTime, const NodeAnim* pNodeAnim);

		static void calcInterpolatedRotation(MQuaternion& Out, float AnimationTime, const NodeAnim* pNodeAnim);

		static void calcInterpolatedScaling(MVector3& Out, float AnimationTime, const NodeAnim* pNodeAnim);
		//------------
		//キーフレームの値を検索し返す------------------------
		static size_t findPosition(float AnimationTime, const NodeAnim* pNodeAnim);

		static size_t findRotation(float AnimationTime, const NodeAnim* pNodeAnim);

		static size_t findScaling(float AnimationTime, const NodeAnim* pNodeAnim);
		//---------------------
		void UpdateNodeMatrix(Bone* pNode);
		void AllNodeMatrixForModelToBone();

		void boneAnimTransform(float& timeInSeconds, Animation* animation = nullptr, bool loop = true, bool animtionPositionRock = false);//Animation前の準備等
	};
}
