#include "PostEffect.h"
#include "KWinApp.h"

KMyMath::Matrix4 PostEffect::matPro;
KGPlin* PostEffect::pipeline = nullptr;
ComPtr<ID3D12Device> PostEffect::device;
ComPtr<ID3D12GraphicsCommandList> PostEffect::cmdList;

void PostEffect::StaticInit()
{
	device = KDirectXCommon::GetInstance()->GetDev();

	cmdList = KDirectXCommon::GetInstance()->GetCmdlist();

	float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	matPro = MyMathUtility::MakeOrthogonalL(0.0f, width, height, 0.0f, 0.0f, 1.0f);
}

void PostEffect::Init()
{
	// 定数バッファマテリアル
	CreateCBMaterial();

	// 頂点、インデックス
	CreateVertexIndex();

	// 定数バッファトランスフォーム
	CreateCBTransform();

	*constMapTransform = MyMathUtility::MakeIdentity();
}

void PostEffect::Update(KMyMath::Vector2 pos, KMyMath::Vector2 scale, float rot, KMyMath::Vector4 color)
{
	// ワールド変換
	KMyMath::Matrix4 matWorld, matTrans, matRot;
	// 移動行列
	matTrans = MyMathUtility::MakeTranslation({ pos.x,pos.y,0.0f });
	// 回転行列
	matRot = MyMathUtility::MakeRotation({ 0.0f,0.0f,XMConvertToRadians(rot) });

	// 全て合体
	matWorld = matRot * matTrans;

	// 定数バッファへ転送
	*constMapTransform = matWorld * matPro;

	// 色の代入
	*constMapMaterial = color;
}

void PostEffect::DrawCommand(KTexture* texture)
{
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
}

void PostEffect::Draw(KTexture* texture, KMyMath::Vector2 pos, KMyMath::Vector2 setSize_, float rot, KMyMath::Vector4 color,
	bool isFlipX_, bool isFlipY_, KMyMath::Vector2 anchorPoint_)
{
	// 非表示処理
	if (isInvisible)
	{
		return;
	}

	if (isFlipX_)
	{
		flipX = -1;
	}
	else
	{
		flipX = 1;
	}

	if (isFlipY_)
	{
		flipY = -1;
	}
	else
	{
		flipY = 1;
	}

	// アンカーポイント
	float left = ((0.0f - anchorPoint_.x) * setSize_.x) * flipX;
	float right = ((1.0f - anchorPoint_.x) * setSize_.x) * flipX;
	float top = ((0.0f - anchorPoint_.y) * setSize_.y) * flipY;
	float bottom = ((1.0f - anchorPoint_.y) * setSize_.y) * flipY;

	// 頂点データ
	Vertex vertices[] =
	{
		{{ left,   top,0.0f},{0.0f,0.0f}}, // 左上
		{{ left,bottom,0.0f},{0.0f,1.0f}}, // 左下
		{{right,   top,0.0f},{1.0f,0.0f}}, // 右上
		{{right,bottom,0.0f},{1.0f,1.0f}}, // 右下
	};

	// インデックスデータ
	uint16_t indices[] =
	{
		1,0,3, // 三角形1つ目
		2,3,0, // 三角形2つ目
	};

	// 全頂点に対して
	std::copy(std::begin(vertices), std::end(vertices), vertMap);

	// 全インデックスに対して
	std::copy(std::begin(indices), std::end(indices), indexMap);

	Update(pos, setSize_, rot, color);

	// パイプラインセット
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト

	// 描画の条件
	DrawCommand(texture);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void PostEffect::SetPipeline(KGPlin* pipeline_)
{
	pipeline = pipeline_;
}

void PostEffect::CreateCBMaterial()
{
	// 定数バッファ生成用
	D3D12_HEAP_PROPERTIES cbHeapProp{}; // ヒープの設定
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
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(constBuffMaterial.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // マッピング
	assert(SUCCEEDED(result));

	// 値の代入
	*constMapMaterial = KMyMath::Vector4(1.0f, 0.5f, 0.5f, 1.0f);
}

void PostEffect::CreateVertexIndex()
{
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
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf())
	);
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
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(indexBuff.ReleaseAndGetAddressOf())
	);
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
	ibView.SizeInBytes = sizeVB;
	assert(SUCCEEDED(result));
}

void PostEffect::CreateCBTransform()
{
	// 定数バッファ生成用
	D3D12_HEAP_PROPERTIES cbHeapProp{}; // ヒープの設定
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
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(constBuffTransform.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // マッピング
	assert(SUCCEEDED(result));
}
