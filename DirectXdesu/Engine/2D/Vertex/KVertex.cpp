#include "KVertex.h"
#include "MyMath.h"

KVertex::KVertex(ID3D12Device* dev, std::vector<VertexPosNormalUV>& vertices, std::vector<unsigned short>& indices) 
{
	KVertexInit(dev, vertices, indices);
}

void KVertex::KVertexInit(ID3D12Device* dev, std::vector<VertexPosNormalUV>& vertices, std::vector<unsigned short>& indices) 
{
#pragma region 頂点
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUV) * vertices.size());

	CD3DX12_HEAP_PROPERTIES heap1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	CD3DX12_RESOURCE_DESC vB = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	result = dev->CreateCommittedResource(
		&heap1,
		D3D12_HEAP_FLAG_NONE,
		&vB,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリを取得
	VertexPosNormalUV* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	std::copy(vertices.begin(), vertices.end(), vertMap);
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
#pragma endregion

#pragma region インデックス
	// インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	CD3DX12_RESOURCE_DESC iB = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	result = dev->CreateCommittedResource(
		&heap1,
		D3D12_HEAP_FLAG_NONE,
		&iB,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// 全インデックスに対して
	std::copy(indices.begin(), indices.end(), indexMap);
	// マッピング解除
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
#pragma endregion

#pragma region 法線の計算
	for (int i = 0; i < indices.size() / 3; i++) 
	{
		// 三角形１つごとに計算
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		// 三角形を構成する頂点座標をベクトルに代入
		DirectX::XMFLOAT3 pV0 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices0].pos);
		DirectX::XMVECTOR p0 = XMLoadFloat3(&pV0);
		DirectX::XMFLOAT3 pV1 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices1].pos);
		DirectX::XMVECTOR p1 = XMLoadFloat3(&pV1);
		DirectX::XMFLOAT3 pV2 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices2].pos);
		DirectX::XMVECTOR p2 = XMLoadFloat3(&pV2);
		// p0 → p1ベクトル、p0 → p2ベクトルを計算 (ベクトルの減算)
		DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
		DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);
		// 外積は両方から垂直なベクトル
		DirectX::XMVECTOR normal = DirectX::XMVector3Cross(v1, v2);
		// 正規化(長さを１にする)
		normal = DirectX::XMVector3Normalize(normal);
		// 求めた法線を頂点データに代入
		DirectX::XMFLOAT3 nV0 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices0].normal);
		DirectX::XMStoreFloat3(&nV0, normal);
		DirectX::XMFLOAT3 nV1 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices1].normal);
		DirectX::XMStoreFloat3(&nV1, normal);
		DirectX::XMFLOAT3 nV2 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices2].normal);
		DirectX::XMStoreFloat3(&nV2, normal);
	}
#pragma endregion
}