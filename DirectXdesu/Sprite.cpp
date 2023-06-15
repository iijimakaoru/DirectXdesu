#include "Sprite.h"
#include "KDirectXCommon.h"
#include "KWinApp.h"

void SpriteCommon::Init()
{
	device = KDirectXCommon::GetInstance()->GetDev();
}

void SpriteCommon::Update()
{
	
}

void SpriteCommon::Draw()
{
	// パイプラインセット
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
}

SpriteCommon* SpriteCommon::GetInstance()
{
	static SpriteCommon instance;
	return &instance;
}

void SpriteCommon::SetPipeline(KGPlin* pipeline_)
{
	pipeline = pipeline_;
}

void Sprite::CreateCBMaterial()
{
	// 定数バッファ生成用
	D3D12_HEAP_PROPERTIES cbHeapProp{}; // ヒープの設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff; // 256バイトアライメント
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
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // マッピング
	assert(SUCCEEDED(result));

	// 値の代入
	constMapMaterial->color = DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 0.5f);
}

void Sprite::CreateCBTransform()
{
	// 定数バッファ生成用
	D3D12_HEAP_PROPERTIES cbHeapProp{}; // ヒープの設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(KConstBufferDataTransform) + 0xff) & ~0xff; // 256バイトアライメント
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
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // マッピング
	assert(SUCCEEDED(result));

	// 単位行列を代入
	constMapTransform->mat = XMMatrixIdentity();
	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;
}

void Sprite::Init()
{
	spriteCommon = SpriteCommon::GetInstance();

	device = KDirectXCommon::GetInstance()->GetDev();

	// 頂点データ
	vertices =
	{
		{{  0.0f,100.0f,0.0f},{0.0f,1.0f}}, // 左下
		{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}}, // 左上
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}}, // 右下
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}}, // 右上
	};

	// 頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転換

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
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
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	// 定数バッファマテリアル
	CreateCBMaterial();

	// 定数バッファトランスフォーム
	CreateCBTransform();
}

void Sprite::Draw(KTexture* texture)
{
	if (isInvisible)
	{
		return;
	}

	spriteCommon->Draw();

	ID3D12GraphicsCommandList* cmdList = KDirectXCommon::GetInstance()->GetCmdlist();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { texture->srvHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// SRVヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texture->srvHeap->GetGPUDescriptorHandleForHeapStart();
	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// 定数バッファビュー(CBV)の設定コマンド(マテリアル)
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// 定数バッファビュー(CBV)の設定コマンド(トランスフォーム)
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// 描画コマンド
	cmdList->DrawInstanced(vertices.size(), 1, 0, 0);
}

void Sprite::Update()
{
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// アンカーポイント
	float left =   (0.0f - anchorPoint.x) * size_.x;
	float right =  (1.0f - anchorPoint.x) * size_.x;
	float top =    (0.0f - anchorPoint.y) * size_.y;
	float bottom = (1.0f - anchorPoint.y) * size_.y;

	// 左右反転
	if (isFlipX)
	{
		left = -left;
		right = -right;
	}
	// 上下反転
	if (isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	vertices[LB].pos = {  left, bottom, 0.0f };
	vertices[LT].pos = {  left,	   top, 0.0f };
	vertices[RB].pos = { right, bottom, 0.0f };
	vertices[RT].pos = { right,	   top, 0.0f };

	// 全頂点に対して
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// ワールド変換
	DirectX::XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	// 回転
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	// 平行移動
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// 定数バッファへ転送
	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;
	constMapTransform->mat *= matWorld;

	// 色の代入
	constMapMaterial->color = color;
}
