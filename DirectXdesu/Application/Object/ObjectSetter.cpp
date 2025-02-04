#include "ObjectSetter.h"

void ObjectSetter::Init(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	centerModel_ = std::make_unique<MeshModel>("centerObject");

	centerObject_ = std::make_unique<CenterObject>();
	centerObject_->Init(centerModel_.get(), timer, matView, matProjection);

	sideModel_ = std::make_unique<MeshModel>("SideObject");

	leftSide_ = -150.0f;
	rightSide_ = 150.0f;

	for (size_t i = 0; i < maxObjectNum; i++)
	{
		// 左側
		{
			// 初期化
			sideObjects_[i] = std::make_unique<SideObject>();
			sideObjects_[i]->Init(sideModel_.get(), timer, matView, matProjection);

			// 初期セット
			KMyMath::Vector3 pos = { leftSide_ + MyMathUtility::GetRandF(-1.0f,1.0f),MyMathUtility::GetRandF(25.0f,100.0f) ,200.0f * (i + 2) };
			float scaleNum = MyMathUtility::GetRandF(10.0f, 20.0f);
			KMyMath::Vector3 scale = { scaleNum,scaleNum ,scaleNum };
			KMyMath::Vector3 rot = { MyMathUtility::GetRandF(0.0f,180.0f),MyMathUtility::GetRandF(0.0f,180.0f) ,MyMathUtility::GetRandF(0.0f,180.0f) };
			KMyMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f };
			sideObjects_[i]->SetObject(pos, rot, scale, color);
		}

		// 右側
		{
			// 初期化
			rightObjects_[i] = std::make_unique<SideObject>();
			rightObjects_[i]->Init(sideModel_.get(), timer, matView, matProjection);

			// 初期セット
			KMyMath::Vector3 pos = { rightSide_ + MyMathUtility::GetRandF(-1.0f,1.0f),MyMathUtility::GetRandF(25.0f,100.0f) ,200.0f * (i + 2) };
			float scaleNum = MyMathUtility::GetRandF(10.0f, 20.0f);
			KMyMath::Vector3 scale = { scaleNum,scaleNum ,scaleNum };
			KMyMath::Vector3 rot = { MyMathUtility::GetRandF(0.0f,180.0f),MyMathUtility::GetRandF(0.0f,180.0f) ,MyMathUtility::GetRandF(0.0f,180.0f) };
			KMyMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f };
			rightObjects_[i]->SetObject(pos, rot, scale, color);
		}
	}
}

void ObjectSetter::Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	centerObject_->Update(timer, matView, matProjection);

	for (std::unique_ptr<SideObject>& sideObject : sideObjects_)
	{
		if (sideObject->GetIsDead()) 
		{
			KMyMath::Vector3 pos = { leftSide_ + MyMathUtility::GetRandF(-1.0f,1.0f),MyMathUtility::GetRandF(-1.0f,1.0f) ,900.0f };
			float scaleNum = MyMathUtility::GetRandF(10.0f, 20.0f);
			KMyMath::Vector3 scale = { scaleNum,scaleNum ,scaleNum };
			KMyMath::Vector3 rot = { MyMathUtility::GetRandF(-180.0f,180.0f),MyMathUtility::GetRandF(-180.0f,180.0f) ,MyMathUtility::GetRandF(-180.0f,180.0f) };
			KMyMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f };

			sideObject->SetObject(pos, rot, scale, color);
		}

		sideObject->Update(timer, matView, matProjection);
	}

	for (std::unique_ptr<SideObject>& sideObject : rightObjects_)
	{
		if (sideObject->GetIsDead())
		{
			KMyMath::Vector3 pos = { rightSide_ + MyMathUtility::GetRandF(-1.0f,1.0f),MyMathUtility::GetRandF(-1.0f,1.0f) ,900.0f };
			float scaleNum = MyMathUtility::GetRandF(10.0f, 20.0f);
			KMyMath::Vector3 scale = { scaleNum,scaleNum ,scaleNum };
			KMyMath::Vector3 rot = { MyMathUtility::GetRandF(-180.0f,180.0f),MyMathUtility::GetRandF(-180.0f,180.0f) ,MyMathUtility::GetRandF(-180.0f,180.0f) };
			KMyMath::Vector4 color = { 1.0f,1.0f ,1.0f ,1.0f };

			sideObject->SetObject(pos, rot, scale, color);
		}

		sideObject->Update(timer, matView, matProjection);
	}
}

void ObjectSetter::Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection)
{
	centerObject_->Draw(timer, matView, matProjection);

	for (std::unique_ptr<SideObject>& sideObject : sideObjects_)
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
	sideObjects_[i]->SetObject(pos, rotation, scale, color);
}
