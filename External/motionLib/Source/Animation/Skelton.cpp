#include <Animation/Skelton.h>
#include <Math/MVector3.h>


using namespace MCBM;
using namespace std;


float clamp(float f)
{
	return (f < 0.0f) ? 0.0f : ((f > 1.0f) ? 1.0f : f);
}

void MCBM::Skelton::SetCapturePtr(Capture* cap)
{
	capture = cap;
}

void MCBM::Skelton::AddBone(std::unique_ptr<Bone> bone)
{
	bones_.push_back(std::move(bone));
}

MCBM::Bone* MCBM::Skelton::GetBone(std::string name)
{
	for (auto& bone : bones_)
	{
		if (bone->GetName() == name)
		{
			return bone.get();
		}
	}
	return nullptr;
}

MCBM::MQuaternion MCBM::Skelton::GetBoneRotation(std::string name)
{
	return GetBone(name)->GetRotation();
}

void MCBM::Skelton::UpdateCaptureData()
{
	capture->Update();
}

void MCBM::Skelton::CaptureBasePoseInitialize()
{
	capture->SetInitialPose();

}

void MCBM::Skelton::SetRootBone(Bone* bone)
{
	rootBone_ = bone;
}

void MCBM::Skelton::CaptureBoneAccept()
{



}

void MCBM::Skelton::Finalize()
{
	capture->Finalize();
}

void MCBM::Skelton::CaptureBoneUpdate(YOLO_POSE_INDEX rootBoneName, uint32_t boneCount)
{

	CaptureData rootCap = capture->GetCaptureData(rootBoneName);
	MVector3 tempVec;
	for (int i = 0; i < boneCount; i++)
	{
		Bone* rootBone = GetBone(rootCap.captureBoneName);
		for (int k = 0; k < 1; k++)
		{
			CaptureData* child = rootCap.captureChildren[k];
			MVector3 initializeBone = MVector3(rootCap.initializedCaptureBonePos, child->initializedCaptureBonePos);
			MVector3 nowBone = MVector3(rootCap.captureBonePos, child->captureBonePos);

			initializeBone.V3Norm();

			nowBone.V3Norm();

			if (i != 0)
			{
				initializeBone = tempVec;
			}

			tempVec = nowBone;

			MVector3 axis = nowBone.GetV3Cross(initializeBone);
			float dotRadian = nowBone.GetV3Dot(initializeBone);
			float rotation = acos(dotRadian);

			if (!isfinite(rotation))
			{
				rotation = 0;
			}
			axis.V3Norm();

			MQuaternion q(axis, rotation);
			q.Normalize();
			rootBone->SetRotation(rootBone->GetInitializeRotation());
			MQuaternion temp = q.GetDirectProduct(rootBone->GetRotation(), q);
			temp.Normalize();
			rootBone->SetRotation(temp);
		}
		rootCap = *rootCap.captureChildren[0];


	}
}



void Skelton::readAnimNodeHeirarchy(float animationTime, Bone* pNode, bool& animationPositionRock, Animation* currentAnimationPtr)
{
	const string& nodeName = pNode->GetName();

	const Animation* pAnimation = currentAnimationPtr;

	Matrix nodeTrans = pNode->GetLocalTransformMatrix();

	const NodeAnim* pNodeAnim = findNodeAnim(pAnimation, nodeName);

	//XMMATRIX mat;
	if (pNodeAnim)
	{
		// //Interpolate scaling and generate scaling transformation matrix
		MVector3 scaling;
		calcInterpolatedScaling(scaling, animationTime, pNodeAnim);
		Matrix scalingM = Matrix::MatrixScaling(scaling);

		// Interpolate rotation and generate rotation transformation matrix
		MQuaternion rotationQ;
		calcInterpolatedRotation(rotationQ, animationTime, pNodeAnim);
		Matrix rotationM = Matrix::GetQuaternionRotaMat(rotationQ);

		// Interpolate translation and generate translation transformation matrix
		MVector3 translation = { 0,0,0 };
		calcInterpolatedPosition(translation, animationTime, pNodeAnim);
		if (animationPositionRock && translation != MVector3(0, 0, 0))
		{
			animationPositionRock = false;
			translation = { 0,0,0 };
			currentAnimationPtr->name = pNodeAnim->name;
		}

		Matrix translationM = Matrix::MatrixTranslate(translation);
		//Combine the above transformations
		pNode->SetTranslation(translation);
		pNode->SetScale(scaling);
		pNode->SetRotation(rotationQ);

		nodeTrans = scalingM * rotationM * translationM;

	}
	if (pNode->GetParent())
	{
		pNode->SetAnimationParentMatrix(nodeTrans * (pNode->GetParent()->GetAnimationMatrix()));
	}
	else
	{
		pNode->SetAnimationParentMatrix(nodeTrans);
	}

}



const NodeAnim* Skelton::findNodeAnim(const Animation* pAnimation, const std::string& NodeName)
{
	if (!pAnimation) return nullptr;
	for (uint32_t i = 0; i < pAnimation->channels.size(); i++)
	{
		const NodeAnim* pNodeAnim = &pAnimation->channels[i];

		if (pNodeAnim->name == NodeName)
		{
			return pNodeAnim;
		}
	}

	return NULL;
}



void Skelton::calcInterpolatedPosition(MVector3& Out, float AnimationTime, const NodeAnim* pNodeAnim)
{
	if (pNodeAnim->position.size() == 1)
	{
		Out = pNodeAnim->position[0];
		return;
	}

	uint32_t PositionIndex = static_cast<uint32_t>(findPosition(AnimationTime, pNodeAnim));
	uint32_t NextPositionIndex = (PositionIndex + 1);
	if (NextPositionIndex >= pNodeAnim->position.size()) NextPositionIndex = (uint32_t)pNodeAnim->position.size() - 1;
	float DeltaTime = (float)(pNodeAnim->positionTime[NextPositionIndex] - pNodeAnim->positionTime[PositionIndex]);
	float Factor = clamp((AnimationTime - (float)pNodeAnim->positionTime[PositionIndex]) / DeltaTime);
	MVector3 Start = pNodeAnim->position[PositionIndex];
	MVector3 End = pNodeAnim->position[NextPositionIndex];
	MVector3 Delta = End - Start;
	Out = Start + Factor * Delta;
}

void Skelton::calcInterpolatedRotation(MQuaternion& Out, float AnimationTime, const NodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->rotation.size() == 1) {
		Out = pNodeAnim->rotation[0];
		return;
	}

	uint32_t RotationIndex = static_cast<uint32_t>(findRotation(AnimationTime, pNodeAnim));
	uint32_t NextRotationIndex = (RotationIndex + 1);
	if (NextRotationIndex >= pNodeAnim->rotation.size()) NextRotationIndex = (uint32_t)pNodeAnim->rotation.size() - 1;
	float DeltaTime = (float)(pNodeAnim->rotationTime[NextRotationIndex] - pNodeAnim->rotationTime[RotationIndex]);
	float Factor = clamp((AnimationTime - (float)pNodeAnim->rotationTime[RotationIndex]) / DeltaTime);

	const MQuaternion& StartRotationQ = pNodeAnim->rotation[RotationIndex];
	const MQuaternion& EndRotationQ = pNodeAnim->rotation[NextRotationIndex];
	Out = Out.Slerp(StartRotationQ, EndRotationQ, Factor);
	Out.Normalize();
}

void Skelton::calcInterpolatedScaling(MVector3& Out, float AnimationTime, const NodeAnim* pNodeAnim)
{
	if (pNodeAnim->scale.size() == 1) {
		Out = pNodeAnim->scale[0];
		return;
	}

	uint32_t ScalingIndex = static_cast<uint32_t>(findScaling(AnimationTime, pNodeAnim));
	uint32_t NextScalingIndex = (ScalingIndex + 1);
	if (NextScalingIndex >= pNodeAnim->scale.size()) NextScalingIndex = (uint32_t)pNodeAnim->scale.size() - 1;
	float DeltaTime = (float)(pNodeAnim->scaleTime[NextScalingIndex] - pNodeAnim->scaleTime[ScalingIndex]);
	float Factor = clamp((AnimationTime - (float)pNodeAnim->scaleTime[ScalingIndex]) / DeltaTime);
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	MVector3 Start = pNodeAnim->scale[ScalingIndex];
	MVector3 End = pNodeAnim->scale[NextScalingIndex];
	MVector3 Delta = End - Start;
	Out = Start + Factor * Delta;
}

size_t Skelton::findPosition(float AnimationTime, const NodeAnim* pNodeAnim)
{
	for (uint32_t i = 0; i < pNodeAnim->position.size(); i++) {
		if (AnimationTime < (float)pNodeAnim->positionTime[i]) {
			return i;
		}
	}

	assert(0);

	return 0;
}


size_t Skelton::findRotation(float AnimationTime, const NodeAnim* pNodeAnim)
{
	assert(pNodeAnim->rotation.size() > 0);

	for (uint32_t i = 0; i < pNodeAnim->rotation.size() - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->rotationTime[i + 1]) {
			return i;
		}
	}

	assert(0);

	return 0;
}


size_t Skelton::findScaling(float AnimationTime, const NodeAnim* pNodeAnim)
{
	assert(pNodeAnim->scale.size() > 0);

	for (uint32_t i = 0; i < pNodeAnim->scale.size() - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->scaleTime[i + 1]) {
			return i;
		}
	}

	assert(0);

	return 0;
}



void Skelton::UpdateNodeMatrix(Bone* pNode)
{
	Matrix nodeTrans;
	Matrix scalingM = Matrix::MatrixScaling(pNode->GetScale());
	Matrix rotationM = Matrix::GetQuaternionRotaMat(pNode->GetRotation());
	Matrix translationM = Matrix::MatrixTranslate(pNode->GetTranslation());
	nodeTrans = scalingM * rotationM * translationM;
	if (pNode->GetParent())
	{
		pNode->SetAnimationParentMatrix(nodeTrans * (pNode->GetParent()->GetAnimationMatrix()));
	}
	else
	{
		pNode->SetAnimationParentMatrix(nodeTrans);
	}

}
void Skelton::AllNodeMatrixForModelToBone()
{

	for (auto& node : bones_)
	{
		UpdateNodeMatrix(node.get());
	}
	for (auto& node : bones_)
	{
		Matrix mat = node->GetAnimationMatrix();
		const string& nodeName = node->GetName();
		std::list<Bone*> bonePtr{};
		for (auto& itr : bones_)
		{
			Matrix boneOff = itr->GetOffSetMatrix();
			Matrix trans = (boneOff) * (mat);
			itr->SetFinalMatrix(trans);
		}


	}

}

void MCBM::Skelton::boneAnimTransform(float& timeInSeconds, Animation* animation, bool loop, bool animtionPositionRock)
{


	float animationTime = 0;
	if (animation != nullptr)
	{
		if (timeInSeconds >= animation->duration)
		{
			timeInSeconds = 0;
		}


		float ticksPerSecond = (float)(animation->ticksPerSecond != 0 ? animation->ticksPerSecond : 25.0f);
		float timeInTicks = timeInSeconds * ticksPerSecond;
		animationTime = timeInTicks;

		if (loop)
		{
			animationTime = (float)std::fmod(animationTime, animation->duration);
		}
		else
		{
			animationTime = (float)std::fmin(animationTime, animation->duration - 0.0001f);
		}
	}
	for (auto& itr : bones_)
	{
		itr->SetAnimationParentMatrix(Matrix::ReturnMatrixIdentity());
		itr->SetFinalMatrix(itr->GetOffSetMatrix());
	}

	bool rootAnim = false;
	if (animtionPositionRock)
	{
		rootAnim = true;
	}

	AllNodeMatrixForModelToBone();
}
