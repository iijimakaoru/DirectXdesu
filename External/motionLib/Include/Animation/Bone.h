#pragma once
#include <string>
#include <vector>
#include <Math/MVector3.h>
#include <Math/MQuaternion.h>
#include <Math/Matrix.h>

namespace MCBM
{
	class Bone
	{
	private:
		std::string name_;
		int32_t num;

		MVector3 scale_;
		MVector3 translation_;
		MQuaternion rotation_;
		Bone* parent_;
		std::string parentName_;
		std::vector<Bone*> children_;
		std::vector<std::string> childrenNames_;
		
		Matrix localTranform_;
		Matrix modelInverseTransform_;
		Matrix animationParentMatrix_;
		Matrix offsetMatrix_;
		Matrix finalMatrix_;

		MVector3 initializeScale_;
		MQuaternion initializeRotation_;
		MVector3 initializeLocalTranslation_;
		Matrix initializeModelTransform_;
		MVector3 initializeModelTranslation_;

		MVector3 modelTranslate_;

		MVector3 topLimitEulerRadian_;
		MVector3 bottomLimitEulerRadian_;
	public:
		void SetRotation(const MQuaternion& rot);
		void SetName(const std::string& name);
		void SetScale(const MVector3& scale);
		void SetTranslation(const MVector3& translation);
		void SetModelTranslation(const MVector3& translation);
		void SetParent(Bone* parent);

		void AddChild(Bone* child);
		void AddChildName(std::string childName);
		void SetFinalMatrix(const Matrix& matrix);
		void SetOffsetMatrix(const Matrix& matrix);
		void SetAnimationParentMatrix(const Matrix& matrix);
		void SetLocalTransform(const Matrix& matrix);
		void RemoveChild(Bone* child);
		//親のボーンを設定し、設定した親のボーンの子に自身を追加する。
		//また既に親のボーンが設定されていた場合、その親のボーンの子ボーンから自身を削除する。
		void SetParentAndChild(Bone* parent);

		void SetParentName(std::string name);

		//現在のデータを初期値として設定
		void SetInitializeTransformData();

		void SetBoneRotationLimit(MVector3 topEulerLimit, MVector3 bottomEulerLimit);

		Matrix GetAnimationMatrix();
		MQuaternion GetRotation();
		MVector3 GetModelTranslate();
		Bone* GetParent();
		std::vector<Bone*> GetChildren();
		std::vector<std::string>* GetChildrenName();
		MVector3 GetTopLimitEulerRadian();
		MVector3 GetBottomLimitEulerRadian();
		MVector3 GetInitializeModelTranslate();
		MQuaternion GetInitializeRotation();
		std::string GetName();
		std::string GetParentName();
		MVector3 GetScale();
		MVector3 GetTranslation();
		Matrix GetOffSetMatrix();
		Matrix GetFinalMatrix();
		Matrix GetLocalTransformMatrix();

		void UpdateMatrix();
	};
}

