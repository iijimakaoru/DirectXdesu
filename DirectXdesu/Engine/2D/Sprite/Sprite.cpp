#include "Sprite.h"
#include "KDirectXCommon.h"
#include "KWinApp.h"

KMyMath::Matrix4 Sprite::matPro;
Microsoft::WRL::ComPtr<ID3D12Device> Sprite::device;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Sprite::cmdList;

void Sprite::StaticInit() {
	device = KDirectXCommon::GetInstance()->GetDev();

	cmdList = KDirectXCommon::GetInstance()->GetCmdlist();

	float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	matPro = MyMathUtility::MakeOrthogonalL(0.0f, width, height, 0.0f, 0.0f, 1.0f);
}

Sprite* Sprite::Create(KGPlin* pipeline_) {
	Sprite* instance = new Sprite();
	if (instance == nullptr) {
		return nullptr;
	}

	instance->Init();
	instance->SetPipeline(pipeline_);

	return instance;
}

void Sprite::SetPipeline(KGPlin* pipeline_) { pipeline = pipeline_; }

const KMyMath::Vector2 Sprite::GetPos() const { return KMyMath::Vector2(); }

void Sprite::CreateCBMaterial() {
	// 定数バッファ生成用
	D3D12_HEAP_PROPERTIES cbHeapProp{};       // ヒープの設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(KMyMath::Vector4) + 0xff) & ~0xff; // 256バイトアライメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
	    &cbHeapProp, // ヒープ設定
	    D3D12_HEAP_FLAG_NONE,
	    &cbResourceDesc, // リソース設定
	    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	    IID_PPV_ARGS(constBuffMaterial.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // マッピング
	assert(SUCCEEDED(result));

	// 値の代入
	*constMapMaterial = KMyMath::Vector4(1.0f, 0.5f, 0.5f, 1.0f);
}

void Sprite::CreateVertexIndex() {
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの数
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * 4);

	// 頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転換

	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの設定
	result = device->CreateCommittedResource(
	    &heapProp, // ヒープ設定
	    D3D12_HEAP_FLAG_NONE,
	    &resDesc, // リソース設定
	    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	    IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(Vertex);

	// インデックスデータのサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * 6);

	// 頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転換

	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの設定
	result = device->CreateCommittedResource(
	    &heapProp, // ヒープ設定
	    D3D12_HEAP_FLAG_NONE,
	    &resDesc, // リソース設定
	    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	    IID_PPV_ARGS(indexBuff.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	// インデックスバッファのマッピング
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));

	// マッピング解除
	indexBuff->Unmap(0, nullptr);

	// インデックスバッファビューの作成
	// GPU仮想アドレス
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	// 頂点バッファのサイズ
	ibView.SizeInBytes = sizeIB;
	assert(SUCCEEDED(result));
}

void Sprite::CreateCBTransform() {
	// 定数バッファ生成用
	D3D12_HEAP_PROPERTIES cbHeapProp{};       // ヒープの設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC cbResDesc{};
	cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResDesc.Width = (sizeof(KMyMath::Matrix4) + 0xff) & ~0xff; // 頂点データ全体のサイズ
	cbResDesc.Height = 1;
	cbResDesc.DepthOrArraySize = 1;
	cbResDesc.MipLevels = 1;
	cbResDesc.SampleDesc.Count = 1;
	cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
	    &cbHeapProp, // ヒープ設定
	    D3D12_HEAP_FLAG_NONE,
	    &cbResDesc, // リソース設定
	    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	    IID_PPV_ARGS(constBuffTransform.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // マッピング
	assert(SUCCEEDED(result));
}

void Sprite::DrawCommand(TextureData texData) {
	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = {texData.srvHeap.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// SRVヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texData.gpuHandle;
	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// 定数バッファビュー(CBV)の設定コマンド(マテリアル)
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// 定数バッファビュー(CBV)の設定コマンド(トランスフォーム)
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->IASetIndexBuffer(&ibView);
}

void Sprite::Init() {
	// 定数バッファマテリアル
	CreateCBMaterial();

	// 頂点、インデックス
	CreateVertexIndex();

	// 定数バッファトランスフォーム
	CreateCBTransform();

	*constMapTransform = MyMathUtility::MakeIdentity();

	isInvisible = false;
}

void Sprite::Draw(
    TextureData& texData, KMyMath::Vector2 pos, KMyMath::Vector2 setSize_, float rot,
    KMyMath::Vector4 color_, bool isFlipX_, bool isFlipY_, KMyMath::Vector2 anchorPoint_) {
	// 非表示処理
	if (isInvisible) {
		return;
	}

	// X反転
	if (isFlipX_) {
		flipX = -1;
	} else {
		flipX = 1;
	}

	// Y反転
	if (isFlipY_) {
		flipY = -1;
	} else {
		flipY = 1;
	}

	// アンカーポイント
	float left = ((0.0f - anchorPoint_.x) * (texData.width * setSize_.x)) * flipX;
	float right = ((1.0f - anchorPoint_.x) * (texData.width * setSize_.x)) * flipX;
	float top = ((0.0f - anchorPoint_.y) * (texData.height * setSize_.y)) * flipY;
	float bottom = ((1.0f - anchorPoint_.y) * (texData.height * setSize_.y)) * flipY;

	// 頂点データ
	Vertex vertices[] = {
	    {{left, top, 0.0f},     {0.0f, 0.0f}}, // 左上
	    {{left, bottom, 0.0f},  {0.0f, 1.0f}}, // 左下
	    {{right, top, 0.0f},    {1.0f, 0.0f}}, // 右上
	    {{right, bottom, 0.0f}, {1.0f, 1.0f}}, // 右下
	};

	// インデックスデータ
	uint16_t indices[] = {
	    1, 0, 3, // 三角形1つ目
	    2, 3, 0, // 三角形2つ目
	};

	// 全頂点に対して
	memcpy(vertMap, vertices, sizeof(Vertex) * 4);

	// 全インデックスに対して
	memcpy(indexMap, indices, sizeof(uint16_t) * 6);

	Update(pos, setSize_, rot, color_);

	// パイプラインセット
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 描画の条件
	DrawCommand(texData);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::DivDraw(
    TextureData& texData, KMyMath::Vector2 leftTop_, KMyMath::Vector2 divSize_,
    KMyMath::Vector2 pos, KMyMath::Vector2 setSize_, float rot, KMyMath::Vector4 color_,
    KMyMath::Vector2 anchorPoint_) {
	// 非表示処理
	if (isInvisible) {
		return;
	}

	// アンカーポイント
	float left = ((0.0f) * (texData.width * setSize_.x));
	float right = ((1.0f) * (texData.width * setSize_.x));
	float top = ((0.0f) * (texData.height * setSize_.y));
	float bottom = ((1.0f) * (texData.height * setSize_.y));

	// UVデータ
	float texLeft = leftTop_.x / texData.width;
	float texRight = (leftTop_.x + divSize_.x) / texData.width;
	float texTop = leftTop_.y / texData.height;
	float texBottom = (leftTop_.y + divSize_.y) / texData.height;

	// 頂点データ
	Vertex vertices[] = {
	    {{left, top, 0.0f},     {texLeft, texTop}    }, // 左上
	    {{left, bottom, 0.0f},  {texLeft, texBottom} }, // 左下
	    {{right, top, 0.0f},    {texRight, texTop}   }, // 右上
	    {{right, bottom, 0.0f}, {texRight, texBottom}}, // 右下
	};

	// インデックスデータ
	uint16_t indices[] = {
	    1, 0, 3, // 三角形1つ目
	    2, 3, 0, // 三角形2つ目
	};

	// 全頂点に対して
	memcpy(vertMap, vertices, sizeof(Vertex) * 4);

	// 全インデックスに対して
	memcpy(indexMap, indices, sizeof(uint16_t) * 6);

	Update(pos, setSize_, rot, color_);

	// パイプラインセット
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 描画の条件
	DrawCommand(texData);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::Update(
    KMyMath::Vector2 pos, KMyMath::Vector2 scale, float rot, KMyMath::Vector4 color_) {
	// ワールド変換
	KMyMath::Matrix4 matWorld, matTrans, matRot;
	// 移動行列
	matTrans = MyMathUtility::MakeTranslation({pos.x, pos.y, 0.0f});
	// 回転行列
	matRot = MyMathUtility::MakeRotation({0.0f, 0.0f, DirectX::XMConvertToRadians(rot)});

	// 全て合体
	matWorld = matRot * matTrans;

	// 定数バッファへ転送
	*constMapTransform = matWorld * matPro;

	// 色の代入
	*constMapMaterial = color_;
}