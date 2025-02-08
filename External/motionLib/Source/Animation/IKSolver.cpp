#include <Animation/IKSolver.h>
#include <Math/MathUtil.h>

void MCBM::IKSolver::CCDIK()
{
	bool run = true;
	bool remaining = false;
	int32_t iteration = iteration_;
	Bone* effectorB = effector_;
	Bone* effectorP = effectorB->GetParent();
	
	for (int32_t i = 0; i < linkBoneCount_; i++)
	{
		if (effectorP == nullptr)
		{
			break;
		}

		effectorB->SetModelTranslation(effectorB->GetInitializeModelTranslate());
		effectorP->SetModelTranslation(effectorP->GetInitializeModelTranslate());

		effectorB->SetRotation(effectorB->GetInitializeRotation());
		effectorP->SetRotation(effectorP->GetInitializeRotation());

		effectorB = effectorP;
		effectorP = effectorB->GetParent();
	}

	while (iteration-- && run)
	{
		MQuaternion idealRotation;
		MQuaternion realRotation;
		MQuaternion remainingRotation;

		MVector3 localTargetPos = targetPos_;


		Bone* effectorBone = effector_;
		Bone* effectorParent = effectorBone->GetParent();
		Bone* rootBone = nullptr;

		MVector3 localEffectorPos = effectorBone->GetModelTranslate();
		MVector3 localEffectorParentPos = effectorParent->GetModelTranslate();

		for (int32_t i = 0; i < linkBoneCount_; i++)
		{

			localEffectorPos = effectorBone->GetModelTranslate();
			localEffectorParentPos = effectorParent->GetModelTranslate();

			//óùëzâÒì]çÏê¨
			MVector3 boneVec = MVector3(localEffectorParentPos, localEffectorPos);
			MVector3 effectToTarget = MVector3(localEffectorParentPos, localTargetPos);

			boneVec.V3Norm();
			effectToTarget.V3Norm();

			MVector3 axis = boneVec.GetV3Cross(effectToTarget);
			float dotRadian = effectToTarget.GetV3Dot(boneVec);
			float radian = MathUtil::Clamp(acos(dotRadian),-1,1);

			if (radian < threshold_)
			{
				continue;
			}

			idealRotation.SetRota(axis, radian);

			idealRotation = idealRotation.GetDirectProduct(idealRotation, effectorParent->GetRotation());

			if (remaining)
			{
				idealRotation = idealRotation.GetDirectProduct(idealRotation, remainingRotation);
			}

			if (isLimit_)
			{
				MVector3 eulerRot = Matrix::GetQuaternionRotaMat(idealRotation).GetRotationToEuler();

				eulerRot = eulerRot.Clamp(eulerRot, effectorParent->GetBottomLimitEulerRadian(),
					effectorParent->GetTopLimitEulerRadian(), &remaining);

				realRotation = realRotation.SetToRorateObjectToInternal(eulerRot);

				remainingRotation = realRotation.GetDirectProduct(
					realRotation.GetConjugated(realRotation), idealRotation);

			}
			else
			{
				realRotation = idealRotation;
			}
			realRotation.Normalize();
			effectorParent->SetRotation(realRotation);

			std::vector<Bone*> nodes;
			nodes.push_back(effectorParent);
			int32_t count = 0;
			while (count < nodes.size())
			{
				nodes[count]->UpdateMatrix();
				for (auto child : nodes[count]->GetChildren())
				{
					nodes.push_back(child);
				}
				count++;
			}
			

			effectorParent = effectorParent->GetParent();
			if (effectorParent == nullptr)
			{
				break;
			}
			else
			{
				rootBone = effectorParent;
			}
		}


		if (rootBone)
		{
			std::vector<Bone*> nodes;
			nodes.push_back(rootBone);
			int32_t count = 0;
			while (count < nodes.size())
			{
				nodes[count]->UpdateMatrix();
				for (auto child : nodes[count]->GetChildren())
				{
					nodes.push_back(child);
				}
				count++;
			}
		}
	}
}

void MCBM::IKSolver::SetIKConfig(int32_t linkBone, int32_t iteration, bool isLimit, bool isCoordinateTransformation, float threshold)
{
	linkBoneCount_ = linkBone;
	iteration_ = iteration;
	isLimit_ = isLimit;
	isCoordinateTransformation_ = isCoordinateTransformation;
	threshold_ = threshold;

}

void MCBM::IKSolver::SetTarget(MVector3 targetPos)
{
	targetPos_ = targetPos;
}
