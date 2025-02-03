#include <Animation/Bone.h>

using namespace MCBM;

void MCBM::Bone::SetRotation(const MQuaternion& rot)
{
	rotation_ = rot;
}

void MCBM::Bone::SetName(const std::string& name)
{
	name_ = name;
}

void MCBM::Bone::SetScale(const MVector3& scale)
{
	scale_ = scale;
}

void MCBM::Bone::SetTranslation(const MVector3& translation)
{
	translation_ = translation;
}

void MCBM::Bone::SetModelTranslation(const MVector3& translation)
{
	modelTranslate_ = translation;
}

void MCBM::Bone::SetParent(Bone* parent)
{
	parent_ = parent;
}

void MCBM::Bone::SetBoneRotationLimit(MVector3 topEulerLimit, MVector3 bottomEulerLimit)
{
	topLimitEulerRadian_ = topEulerLimit;
	bottomLimitEulerRadian_ = bottomEulerLimit;
}

std::string MCBM::Bone::GetName()
{
	return name_;
}

MVector3 MCBM::Bone::GetScale()
{
	return scale_;
}

MVector3 MCBM::Bone::GetTranslation()
{
	return translation_;
}

Matrix MCBM::Bone::GetOffSetMatrix()
{
	return offsetMatrix_;
}

Matrix MCBM::Bone::GetFinalMatrix()
{
	return finalMatrix_;
}

Matrix MCBM::Bone::GetLocalTransformMatrix()
{
	return localTranform_;
}

void MCBM::Bone::AddChild(Bone* child)
{
	children_.push_back(child);
}

void MCBM::Bone::SetFinalMatrix(const Matrix& matrix)
{
	finalMatrix_ = matrix;
}

void MCBM::Bone::SetOffsetMatrix(const Matrix& matrix)
{
	offsetMatrix_ = matrix;
}

void MCBM::Bone::SetAnimationParentMatrix(const Matrix& matrix)
{
	animationParentMatrix_ = matrix;
}

void MCBM::Bone::RemoveChild(Bone* child)
{
	auto itr = find(children_.begin(), children_.end(), child);
	if (itr != children_.end())
	{
		children_.erase(itr);
	}

}

void MCBM::Bone::SetParentAndChild(Bone* parent)
{
	if (parent_ != nullptr)
	{
		parent_->RemoveChild(this);
	}
	parent_ = parent;
	parent_->AddChild(this);
}

void MCBM::Bone::SetInitializeTransformData()
{
	initializeLocalTranslation_ = translation_;
	initializeRotation_ = rotation_;
	initializeScale_ = scale_;
	UpdateMatrix();
	initializeModelTransform_ = animationParentMatrix_;
	initializeModelTranslation_ = initializeModelTransform_.GetTranslate(initializeModelTransform_);
}

Matrix MCBM::Bone::GetAnimationMatrix()
{
	return animationParentMatrix_;
}

MQuaternion MCBM::Bone::GetRotation()
{
	return rotation_;
}

MVector3 MCBM::Bone::GetModelTranslate()
{
	return modelTranslate_;
}

Bone* MCBM::Bone::GetParent()
{
	return parent_;
}
std::vector<Bone*> MCBM::Bone::GetChildren()
{
	return children_;
}

MVector3 MCBM::Bone::GetTopLimitEulerRadian()
{
	return topLimitEulerRadian_;
}

MVector3 MCBM::Bone::GetBottomLimitEulerRadian()
{
	return bottomLimitEulerRadian_;
}

MVector3 MCBM::Bone::GetInitializeModelTranslate()
{
	return initializeModelTranslation_;
}

MQuaternion MCBM::Bone::GetInitializeRotation()
{
	return initializeRotation_;
}

void MCBM::Bone::UpdateMatrix()
{
	Matrix scaleMat = Matrix::MatrixScaling(scale_);
	Matrix rotationMat = Matrix::GetQuaternionRotaMat(rotation_);
	Matrix translationMat = Matrix::MatrixTranslate(translation_);

	localTranform_ = scaleMat * rotationMat * translationMat;
	if (parent_)
	{
		animationParentMatrix_ = localTranform_ * parent_->GetAnimationMatrix();
	}
	else
	{
		animationParentMatrix_ = localTranform_;
	}

	modelTranslate_ = animationParentMatrix_.GetTranslate(animationParentMatrix_);
}
