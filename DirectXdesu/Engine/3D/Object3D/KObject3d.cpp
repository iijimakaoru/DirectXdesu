#include "KObject3d.h"
#include "KDirectXCommon.h"

std::unique_ptr<Light> KObject3d::light_ = nullptr;

KObject3d::~KObject3d() { Finalize(); }

void KObject3d::StaticInit() {}

KObject3d* KObject3d::Create(KModel* model_, KGPlin* pipeline_) {
	// インスタンス生成
	KObject3d* object3d = new KObject3d();
	if (object3d == nullptr) {
		return nullptr;
	}

	// 初期化
	object3d->LoadModel(model_);
	object3d->SetPipeline(pipeline_);
	object3d->Initialize();

	return object3d;
}

void KObject3d::SetLight(Light* light) { light_.reset(light); }

void KObject3d::Initialize() {
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 定数バッファB0
	// リソース設定
	D3D12_RESOURCE_DESC b0ResourceDesc{};
	b0ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	b0ResourceDesc.Width = (sizeof(ConstBufferDataB0) + 0xff) & ~0xff;
	b0ResourceDesc.Height = 1;
	b0ResourceDesc.DepthOrArraySize = 1;
	b0ResourceDesc.MipLevels = 1;
	b0ResourceDesc.SampleDesc.Count = 1;
	b0ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
	    &heapProp, D3D12_HEAP_FLAG_NONE, &b0ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
	    nullptr, IID_PPV_ARGS(&constBuffB0));
	assert(SUCCEEDED(result));

	model->Init();
}

void KObject3d::LoadModel(KModel* model_) { model = model_; }

void KObject3d::SetPipeline(KGPlin* pipeline_) { pipeline = pipeline_; }

void KObject3d::CreateCBMaterial() {}

void KObject3d::CreateVertexIndex() {}

void KObject3d::CreateCBTransform() {}

void KObject3d::TransUpdate() {
	// マトリックス
	KMyMath::Matrix4 matScale, matRot, matTrans, matWorld;
	matScale = matRot = matTrans = MyMathUtility::MakeIdentity();

	// 親オブジェクト要素
	matScale = MyMathUtility::MakeScaling(transform.GetScale());
	matRot = MyMathUtility::MakeRotation(
	    {DirectX::XMConvertToRadians(transform.GetRot().x),
	     DirectX::XMConvertToRadians(transform.GetRot().y),
	     DirectX::XMConvertToRadians(transform.GetRot().z)});
	matTrans = MyMathUtility::MakeTranslation(transform.GetPos());

	// 行列初期化
	transform.SetMatWorld(MyMathUtility::MakeIdentity());

	matWorld *= MyMathUtility::MakeWorld(matTrans, matScale, matRot);

	if (transform.GetParent()) {
		matWorld *= transform.GetParent()->GetMatWorld();
	}

	//
	transform.SetMatWorld(matWorld);
}

void KObject3d::MatUpdate(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	KMyMath::Vector3 hoge = cameraPos;
	// 定数バッファのマッピング
	// B0
	ConstBufferDataB0* constMap0 = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap0);
	constMap0->color = color;
	constMap0->world = transform.GetMatWorld();
	constMap0->viewPro = viewPro->GetMatView() * viewPro->GetMatPro();
	constMap0->cameraPos = hoge;
	constBuffB0->Unmap(0, nullptr);
	assert(SUCCEEDED(result));
}

void KObject3d::Update(ViewProjection* viewPro, const KMyMath::Vector3& cameraPos) {
	TransUpdate();

	model->Update();

	MatUpdate(viewPro, cameraPos);
}

void KObject3d::Draw() {
	pipeline->Setting();
	pipeline->Update(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 定数バッファビューをセット
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(
	    0, constBuffB0->GetGPUVirtualAddress());

	light_->Draw(3);

	model->Draw(1);
}

void KObject3d::Draw(TextureData& texData_) {
	pipeline->Setting();
	pipeline->Update(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 定数バッファビューをセット
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(
	    0, constBuffB0->GetGPUVirtualAddress());

	light_->Draw(3);

	model->Draw(1, texData_);
}

void KObject3d::SetParent(const Transform* parent_) { transform.SetParent(parent_); }

void KObject3d::Finalize() {  }

Transform& KObject3d::GetTransform() { return transform; }

void KObject3d::SetColor(const KMyMath::Vector4& color_) { color = color_; }

void KObject3d::SetRGB(const KMyMath::Vector3& rgb) {
	float maxNum = 255.0f;
	color.x = rgb.x / maxNum;
	color.y = rgb.y / maxNum;
	color.z = rgb.z / maxNum;
}

void KObject3d::SetAlpha(const float& a) {
	float maxNum = 255.0f;
	color.w = a / maxNum;
}
