#include "BillManager.h"
#include "ModelManager.h"
#include "PipelineManager.h"

BillManager::BillManager() {}

BillManager::~BillManager() {}

void BillManager::Init(Light* light) {
	billtimer = 0;

	isAdvance = false;

	light_ = light;
}

void BillManager::Update(
    ViewProjection* viewPro_, const KMyMath::Vector3& cameraPos, const float& cameraZ_) {
	bill1s.remove_if([](std::unique_ptr<Bill1>& bill1) { return bill1->GetIsDead(); });

	if (!isStopCreate) {
		Set(cameraZ_);
	}

	for (std::unique_ptr<Bill1>& bill1 : bill1s) {
		bill1->Update(viewPro_, cameraPos, cameraZ_, isAdvance);
	}
}

void BillManager::Draw() {
	for (std::unique_ptr<Bill1>& bill1 : bill1s) {
		bill1->Draw();
	}
}

void BillManager::LeftSet(const float cameraZ_) {
	//
	std::unique_ptr<Bill1> newBill1;
	newBill1.reset(Bill1::Create(
	    ModelManager::GetInstance()->GetModels("Bill1"),
	    PipelineManager::GetInstance()->GetPipeline("Obj"), {-60, cameraZ_}));
	bill1s.push_back(std::move(newBill1));
}

void BillManager::SetIsAdvance(const bool isAdvance_) { isAdvance = isAdvance_; }

void BillManager::SetIsStopCreate(const bool isStopCreate_) { isStopCreate = isStopCreate_; }

void BillManager::Set(const float& cameraZ_) {
	billtimer++;

	if (billtimer >= billtime) {
		RightSet(cameraZ_ + 500.0f);
		LeftSet(cameraZ_ + 500.0f);
		billtimer = 0;
	}
}

void BillManager::RightSet(const float cameraZ_) {
	//
	std::unique_ptr<Bill1> newBill1;
	newBill1.reset(Bill1::Create(
	    ModelManager::GetInstance()->GetModels("Bill1"),
	    PipelineManager::GetInstance()->GetPipeline("Obj"), {60, cameraZ_}));
	bill1s.push_back(std::move(newBill1));
}
