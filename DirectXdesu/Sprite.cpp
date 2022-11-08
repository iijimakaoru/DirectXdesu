#include "Sprite.h"
#include "KShader.h"

void Sprite::Init(SpriteCommon* spriteCommon)
{
	assert(spriteCommon);
	spriteCommon_ = spriteCommon;


}

void Sprite::SpriteTransferVertexBuffer()
{
	HRESULT result = S_FALSE;

	VertexPosUV vertices[] = {
		{{},{0.0f,1.0f}}, // 左下
		{{},{0.0f,0.0f}}, // 左上
		{{},{1.0f,1.0f}}, // 右下
		{{},{1.0f,0.0f}}, // 右上
	};

	enum { LB, LT, RB, RT };

	float left = (0.0f - sprite_.anchorpoint.x) * sprite_.size.x;
	float right = (1.0f - sprite_.anchorpoint.x) * sprite_.size.x;
	float top = (0.0f - sprite_.anchorpoint.y) * sprite_.size.y;
	float bottom = (1.0f - sprite_.anchorpoint.y) * sprite_.size.y;

	if (sprite_.isFlipX)
	{
		left = -left;
		right = -right;
	}

	if (sprite_.isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,	  top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,	  top,0.0f };

	if (sprite_.isKiridasi)
	{
		if (spriteCommon_->GetSpriteCommond().texBuff[sprite_.texNum])
		{
			// 
			D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetSpriteCommond().texBuff[sprite_.texNum]->GetDesc();

			float tex_left = sprite_.texLeftTop.x / resDesc.Width;
			float tex_right = (sprite_.texLeftTop.x + sprite_.texSize.x) / resDesc.Width;
			float tex_top = sprite_.texLeftTop.y / resDesc.Height;
			float tex_bottom = (sprite_.texLeftTop.y + sprite_.texSize.y) / resDesc.Height;

			vertices[LB].uv = { tex_left,tex_bottom };
			vertices[LT].uv = { tex_left,tex_top };
			vertices[RB].uv = { tex_right,tex_bottom };
			vertices[RT].uv = { tex_right,tex_top };
		}
	}

	// 頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = sprite_.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite_.vertBuff->Unmap(0, nullptr);
}

SpriteInfo Sprite::SpriteCreate(UINT texNumber,  Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;
	// 新しいスプライトを作る
	SpriteInfo sprite{};
	// 頂点データ
	VertexPosUV vertices[] =
	{
		// x	  y		 z		  u	   v
		{{  0.0f,100.0f,  0.0f},{0.0f,1.0f}}, // 左下 
		{{  0.0f,  0.0f,  0.0f},{0.0f,0.0f}}, // 左上
		{{100.0f,100.0f,  0.0f},{1.0f,1.0f}}, // 右下
		{{100.0f,  0.0f,  0.0f},{1.0f,0.0f}}, // 右上
	};
	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = static_cast<UINT>(sizeof(vertices));
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// テクスチャ番号コピー
	sprite.texNum = texNumber;

	// 頂点バッファ生成
	result = spriteCommon_->GetDev()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.vertBuff));

	// 指定番号の画像が読み込み済みなら
	if (spriteCommon_->GetSpriteCommond().texBuff[sprite.texNum])
	{
		// 
		D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetSpriteCommond().texBuff[sprite.texNum]->GetDesc();
		// 
		sprite.size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// 反転フラグコピー
	sprite.isFlipX = isFlipX;
	sprite.isFlipY = isFlipY;

	// アンカーポイントコピー
	sprite.anchorpoint = anchorpoint;

	// 頂点バッファデータ転送
	SpriteTransferVertexBuffer();

	// 頂点バッファビューの作成
	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);
	// 定数バッファの生成
	result = spriteCommon_->GetDev()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.constBuff)
	);
	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = sprite.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // 色指定
	// 平行投影行列
	constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, KWinApp::window_width, KWinApp::window_height, 0.0f, 0.0f, 1.0f);
	sprite.constBuff->Unmap(0, nullptr);

	return sprite;
}

void Sprite::SpriteDraw()
{
	// 非表示フラグ
	if (sprite_.isInvisible)
	{
		return;
	}
	// 頂点バッファをセット
	spriteCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &sprite_.vbView);
	// 定数バッファをセット
	spriteCommon_->GetCmdList()->SetGraphicsRootConstantBufferView(0, sprite_.constBuff->GetGPUVirtualAddress());
	// シェーダーリソースビューをセット
	spriteCommon_->GetCmdList()->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteCommon_->GetSpriteCommond().descHeap->GetGPUDescriptorHandleForHeapStart(),
			sprite_.texNum,
			spriteCommon_->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	// ポリゴンの描画
	spriteCommon_->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void Sprite::SpriteUpdate()
{
	// ワールド行列
	sprite_.matWorld = XMMatrixIdentity();
	// Z軸回転
	sprite_.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite_.rotation));
	// 平行移動
	sprite_.matWorld *= XMMatrixTranslation(sprite_.position.x, sprite_.position.y, sprite_.position.z);
	// 定数バッファの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = sprite_.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = sprite_.matWorld * spriteCommon_->GetSpriteCommond().matProjection;
	constMap->color = sprite_.color;
	sprite_.constBuff->Unmap(0, nullptr);
}
