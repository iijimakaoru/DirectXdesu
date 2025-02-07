#pragma once
#include <memory>
#include <unordered_map>

#include <Animation/Bone.h>
#include <Animation/CaptureManager.h>
#include <Animation/AnimationManager.h>
#include <Animation/AnimationMesh.h>
#include <P_MODEL_DATA.h>
#include <Animation/ModelOut.h>


namespace MCBM
{
	class Skelton
	{
	private:
		std::vector<std::unique_ptr<Bone>> bones_;

		M_MODEL_OUT modelOut;
		
		AnimationManager animations_;

		Bone* rootBone_;

		std::unique_ptr<MCBM::CaptureManager> captureManager;

		std::string name_;

		Matrix globalInverseTransform;

		bool canAnimation = false;

		const int32_t CAMERA_NUM = 1;
	private:

		void AddBone(std::unique_ptr<Bone> bone);

		void AddMesh(const M_MODEL_MESH& mesh);


		MQuaternion GetBoneRotation(std::string name);

		//キャプチャーデータの読み込み
		void UpdateCaptureData();

		//モデルの初期姿勢の設定（プレイヤーにモデルの素のポーズをまねてもらう）
		void CaptureBasePoseInitialize();

		void SetRootBone(Bone* bone);

		void CaptureBoneAccept();

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
		

	public:

		void Initialize();

		Bone* GetBone(std::string name);

		const MVector3& GetCaptureThreeDimensionPos(YOLO_POSE_INDEX index);

		const MVector3& GetCapturePos(YOLO_POSE_INDEX index,Locate cameraLocate = Locate::FRONT);

		Skelton& SetDataFromLoader(const PHONONLOADER::P_MODEL_DATA& modelData);

		/// <summary>
		/// キャプチャーによる腕の動きやアニメーションの適用
		/// ただし、キャプチャーとアニメーションはどちらかしか有効にできない。
		/// SetStartAnimation()で設定。falseならキャプチャー
		/// </summary>
		/// <param name="rootBoneName">キャプチャーの影響を受けるボーンの根元のボーン</param>
		/// <param name="timeInSeconds"></param>
		/// <param name="currentAnimation"></param>
		/// <param name="loop"></param>
		/// <param name="animtionPositionRock"></param>
		void UpDate(std::vector<YOLO_POSE_INDEX> rootBoneNames,float& timeInSeconds, const std::string& currentAnimation = "Tpose", bool loop = true, bool animtionPositionRock = true);

		void CaptureUpDate(std::vector<YOLO_POSE_INDEX> rootBoneNames);

		void InitializePoseData();

		void Finalize();

		/// <summary>
		///	描画に必要な計算済みデータ
		/// </summary>
		/// <returns></returns>
		const M_MODEL_OUT& GetModelOutData();

		const std::vector<Matrix> GetMeshBoneData(int32_t meshIndex);
	};
}
