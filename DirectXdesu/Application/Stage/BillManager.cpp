#include "BillManager.h"

void BillManager::Init()
{
	model = std::make_unique<MtlObj>("bill1");
	model->CreateModel();

	// シェーダー
	objShader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");

	// パイプライン
	objPipeline.reset(KGPlin::Create(objShader, "Obj"));

	billtimer = 0;
}

void BillManager::Update(ViewProjection* viewPro, const float& cameraZ)
{
	bill1s.remove_if([](std::unique_ptr<Bill1>& bill1) {return bill1->GetIsDead(); });

	for (std::unique_ptr<Bill1>& bill1 : bill1s)
	{
		bill1->Update(viewPro, cameraZ);
	}
}

void BillManager::Draw()
{
	for (std::unique_ptr<Bill1>& bill1 : bill1s)
	{
		bill1->Draw();
	}
}

void BillManager::LeftSet(const float cameraZ)
{
	//
	std::unique_ptr<Bill1> newBill1;
	newBill1.reset(Bill1::Create(model.get(), objPipeline.get(), { -60,cameraZ}));
	bill1s.push_back(std::move(newBill1));
}

void BillManager::Set(const float& cameraZ)
{
	billtimer++;

	if (billtimer >= billtime)
	{
		RightSet(cameraZ + 500.0f);
		LeftSet(cameraZ + 500.0f);
		billtimer = 0;
	}
}

void BillManager::RightSet(const float cameraZ)
{
	//
	std::unique_ptr<Bill1> newBill1;
	newBill1.reset(Bill1::Create(model.get(), objPipeline.get(), { 60,cameraZ}));
	bill1s.push_back(std::move(newBill1));
}
