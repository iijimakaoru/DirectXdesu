#include "Sprite.h"

Sprite::SpriteBuff Sprite::SpriteCreate(ID3D12Device* dev, int window_width, int window_height)
{
	HRESULT result = S_FALSE;
	// 新しいスプライトを作る
	SpriteBuff sprite{};
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
	// 頂点バッファ生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.vertBuff)
	);
	// 頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
	// 頂点バッファビューの作成
	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);
	// 定数バッファの生成
	result = dev->CreateCommittedResource(
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
	constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
	sprite.constBuff->Unmap(0, nullptr);

	return sprite;
}

void Sprite::SpriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdList, const PipelineSet& pipelineSet, ID3D12DescriptorHeap* descHeap)
{
	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineSet.pipelineState.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipelineSet.rootSignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// テクスチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Sprite::SpriteDraw(const SpriteBuff& sprite, ID3D12GraphicsCommandList* cmdList)
{
	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	// 定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, sprite.constBuff->GetGPUVirtualAddress());
	// ポリゴンの描画
	cmdList->DrawInstanced(4, 1, 0, 0);
}
