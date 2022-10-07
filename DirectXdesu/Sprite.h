#pragma once
#include <DirectXMath.h>
#include "KDirectInit.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include "PipelineSet.h"
#include "ConstBuffer.h"

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class Sprite
{
public:
	struct SpriteBuff
	{
		// 頂点バッファ
		ComPtr<ID3D12Resource> vertBuff;
		//	頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		// 定数バッファ
		ComPtr<ID3D12Resource> constBuff;
	};
	SpriteBuff SpriteCreate(ID3D12Device* dev, int window_width, int window_height);
	void SpriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdList, const PipelineSet& pipelineSet, ID3D12DescriptorHeap* descHeap);
	void SpriteDraw(const SpriteBuff& sprite, ID3D12GraphicsCommandList* cmdList);

private:

};

