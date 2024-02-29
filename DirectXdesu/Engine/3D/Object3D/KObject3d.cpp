#include "KObject3d.h"
#include "KDirectXCommon.h"

void KObject3d::StaticInit() {}

KObject3d* KObject3d::Create(KModel* model_, KGPlin* pipeline_) {
	// インスタンス生成
	KObject3d* object3d = new KObject3d();
	if (object3d == nullptr) {
		return nullptr;
	}

	// 初期化
	object3d->Initialize();
	object3d->LoadModel(model_);
	object3d->SetPipeline(pipeline_);

	return object3d;
}

void KObject3d::Initialize() {
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 定数バッファB1
	// リソース設定
	D3D12_RESOURCE_DESC b1ResourceDesc{};
	b1ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	b1ResourceDesc.Width = (sizeof(ConstBufferDataB1) + 0xff) & ~0xff;
	b1ResourceDesc.Height = 1;
	b1ResourceDesc.DepthOrArraySize = 1;
	b1ResourceDesc.MipLevels = 1;
	b1ResourceDesc.SampleDesc.Count = 1;
	b1ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
	    &heapProp, D3D12_HEAP_FLAG_NONE, &b1ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
	    nullptr, IID_PPV_ARGS(&constBuffB1));
	assert(SUCCEEDED(result));

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
}

void KObject3d::LoadModel(KModel* model_) { model = model_; }

void KObject3d::SetPipeline(KGPlin* pipeline_) { pipeline = pipeline_; }

void KObject3d::CreateCBMaterial() {}

void KObject3d::CreateVertexIndex() {}

void KObject3d::CreateCBTransform() {}

void KObject3d::TransUpdate() {
	// マトリックス
	KMyMath::Matrix4 matScale, matRot, matTrans;
	matScale = matRot = matTrans = MyMathUtility::MakeIdentity();

	// 親オブジェクト要素
	matScale = MyMathUtility::MakeScaling(transform.scale);
	matRot = MyMathUtility::MakeRotation(
	    {DirectX::XMConvertToRadians(transform.rot.x), DirectX::XMConvertToRadians(transform.rot.y),
	     DirectX::XMConvertToRadians(transform.rot.z)});
	matTrans = MyMathUtility::MakeTranslation(transform.pos);

	// 行列初期化
	transform.matWorld = MyMathUtility::MakeIdentity();
	//
	transform.matWorld *= MyMathUtility::MakeWorld(matTrans, matScale, matRot);

	if (transform.parent) {
		transform.matWorld *= transform.parent->matWorld;
	}
}

void KObject3d::MatUpdate(ViewProjection* viewProjection_) {
	// 定数バッファのマッピング
	// B1
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = model->objMtl.ambient;
	constMap1->diffuse = model->objMtl.diffuse;
	constMap1->specular = model->objMtl.specular;
	constMap1->alpha = model->objMtl.alpha;
	constBuffB1->Unmap(0, nullptr);
	assert(SUCCEEDED(result));
	// B0
	ConstBufferDataB0* constMap0 = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap0);
	constMap0->mat =
	    transform.matWorld * viewProjection_->GetMatView() * viewProjection_->GetMatPro();
	constMap0->color = color;
	constBuffB0->Unmap(0, nullptr);
	assert(SUCCEEDED(result));
}

void KObject3d::Update(ViewProjection* viewProjection_) {
	TransUpdate();

	MatUpdate(viewProjection_);
}

void KObject3d::Draw() {
	pipeline->Setting();
	pipeline->Update(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 定数バッファビューをセット
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(
	    1, constBuffB0->GetGPUVirtualAddress());
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(
	    2, constBuffB1->GetGPUVirtualAddress());

	model->Draw();
}

void KObject3d::Draw(TextureData& texData_) {
	pipeline->Setting();
	pipeline->Update(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 定数バッファビューをセット
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(
	    1, constBuffB0->GetGPUVirtualAddress());
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(
	    2, constBuffB1->GetGPUVirtualAddress());

	model->Draw(texData_);
}

void KObject3d::SetParent(const WorldTransfom* parent_) { transform.parent = parent_; }

void KObject3d::Finalize() {}

const KMyMath::Vector3& KObject3d::GetPos() const { return transform.pos; }

const KMyMath::Vector3& KObject3d::GetRot() const { return transform.rot; }

const KMyMath::Vector3& KObject3d::GetScale() const { return transform.scale; }

const KMyMath::Matrix4& KObject3d::GetMatWorld() const { return transform.matWorld; }

const WorldTransfom& KObject3d::GetTransform() const { return transform; }

void KObject3d::SetPos(const KMyMath::Vector3& pos_) { transform.pos = pos_; }

void KObject3d::SetRot(const KMyMath::Vector3& rot_) { transform.rot = rot_; }

void KObject3d::SetScale(const KMyMath::Vector3& scale_) { transform.scale = scale_; }

void KObject3d::AddSetPos(const KMyMath::Vector3& pos_) { transform.pos += pos_; }

void KObject3d::AddSetRot(const KMyMath::Vector3& rot_) { transform.rot += rot_; }

void KObject3d::AddSetScale(const KMyMath::Vector3& scale_) { transform.scale += scale_; }

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
