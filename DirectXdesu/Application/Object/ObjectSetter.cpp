#include "ObjectSetter.h"

void ObjectSetter::Init(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	centerModel_ = std::make_unique<MeshModel>("centerObject");

	centerObject_ = std::make_unique<CenterObject>();
	centerObject_->Init(centerModel_.get(), timer, matView, matProjection);

	sideModel_ = std::make_unique<MeshModel>("SideObject");

	leftSide_ = -180.0f;
	rightSide_ = 180.0f;

	{
		KMyMath::Vector3 centerObjectPos = { 0.0f,100.0f,800.0f };
		KMyMath::Vector3 centerObjectRot = { 0.0f,0.0f,0.0f };
		KMyMath::Vector3 centerObjectScale = { 1.0f,1.0f,1.0f };
		KMyMath::Vector4 centerObjectColor = { 1.0f,1.0f,1.0f,1.0f };
		centerObject_->SetObject(centerObjectPos, centerObjectRot, centerObjectScale, centerObjectColor);
	}

	for (size_t i = 0; i < maxObjectNum; i++)
	{
		// 左側
		{
			// 初期化
			leftObjects_[i] = std::make_unique<SideObject>();
			leftObjects_[i]->Init(sideModel_.get(), timer, matView, matProjection);

			// 初期セット
			KMyMath::Vector3 pos = { leftSide_ + MyMathUtility::GetRandF(-15.0f,15.0f),MyMathUtility::GetRandF(25.0f,100.0f) ,200.0f * (i + 2) };
			float scaleNum = MyMathUtility::GetRandF(10.0f, 20.0f);
			KMyMath::Vector3 scale = { scaleNum,scaleNum ,scaleNum };
			KMyMath::Vector3 rot = { MyMathUtility::GetRandF(0.0f,180.0f),MyMathUtility::GetRandF(0.0f,180.0f) ,MyMathUtility::GetRandF(0.0f,180.0f) };
			KMyMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f };
			leftObjects_[i]->SetObject(pos, rot, scale, color);
		}

		// 右側
		{
			// 初期化
			rightObjects_[i] = std::make_unique<SideObject>();
			rightObjects_[i]->Init(sideModel_.get(), timer, matView, matProjection);

			// 初期セット
			KMyMath::Vector3 pos = { rightSide_ + MyMathUtility::GetRandF(-15.0f,15.0f),MyMathUtility::GetRandF(25.0f,100.0f) ,200.0f * (i + 2) };
			float scaleNum = MyMathUtility::GetRandF(10.0f, 20.0f);
			KMyMath::Vector3 scale = { scaleNum,scaleNum ,scaleNum };
			KMyMath::Vector3 rot = { MyMathUtility::GetRandF(0.0f,180.0f),MyMathUtility::GetRandF(0.0f,180.0f) ,MyMathUtility::GetRandF(0.0f,180.0f) };
			KMyMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f };
			rightObjects_[i]->SetObject(pos, rot, scale, color);
		}
	}
}

void ObjectSetter::Update(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	centerObject_->Update(timer, matView, matProjection);

	for (std::unique_ptr<SideObject>& sideObject : leftObjects_)
	{
		if (sideObject->GetIsDead()) 
		{
			KMyMath::Vector3 pos = { leftSide_ + MyMathUtility::GetRandF(-10.0f,10.0f),MyMathUtility::GetRandF(25.0f,100.0f) ,900.0f };
			float scaleNum = MyMathUtility::GetRandF(10.0f, 20.0f);
			KMyMath::Vector3 scale = { scaleNum,scaleNum ,scaleNum };
			KMyMath::Vector3 rot = { MyMathUtility::GetRandF(0.0f,180.0f),MyMathUtility::GetRandF(0.0f,180.0f) ,MyMathUtility::GetRandF(0.0f,180.0f) };
			KMyMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f };

			sideObject->SetObject(pos, rot, scale, color);
		}

		sideObject->Update(timer, matView, matProjection);
	}

	for (std::unique_ptr<SideObject>& sideObject : rightObjects_)
	{
		if (sideObject->GetIsDead())
		{
			KMyMath::Vector3 pos = { rightSide_ + MyMathUtility::GetRandF(-10.0f,10.0f),MyMathUtility::GetRandF(25.0f,100.0f) ,900.0f };
			float scaleNum = MyMathUtility::GetRandF(10.0f, 20.0f);
			KMyMath::Vector3 scale = { scaleNum,scaleNum ,scaleNum };
			KMyMath::Vector3 rot = { MyMathUtility::GetRandF(0.0f,180.0f),MyMathUtility::GetRandF(0.0f,180.0f) ,MyMathUtility::GetRandF(0.0f,180.0f) };
			KMyMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f };

			sideObject->SetObject(pos, rot, scale, color);
		}

		sideObject->Update(timer, matView, matProjection);
	}
}

void ObjectSetter::Draw(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	centerObject_->Draw(timer, matView, matProjection);

	for (std::unique_ptr<SideObject>& sideObject : leftObjects_)
	{
		sideObject->Draw(timer, matView, matProjection);
	}

	for (std::unique_ptr<SideObject>& sideObject : rightObjects_)
	{
		sideObject->Draw(timer, matView, matProjection);
	}
}

void ObjectSetter::SetCenterObject(KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color)
{
	centerObject_->SetObject(pos, rotation, scale, color);
}

void ObjectSetter::SetSideObject(size_t& i, KMyMath::Vector3& pos, KMyMath::Vector3& rotation, KMyMath::Vector3& scale, KMyMath::Vector4& color)
{
	leftObjects_[i]->SetObject(pos, rotation, scale, color);
}

void ObjectSetter::SetSideVibration()
{
	for (std::unique_ptr<SideObject>& sideObject : leftObjects_)
	{
		sideObject->SetVibration();
	}

	for (std::unique_ptr<SideObject>& sideObject : rightObjects_)
	{
		sideObject->SetVibration();
	}
}
