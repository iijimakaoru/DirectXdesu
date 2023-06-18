#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Vector3.h"
#include "KModel.h"
#include "KTexture.h"
#include "ViewProjection.h"
#include "KGPlin.h"

using namespace DirectX;

struct ConstBufferDataB1
{
	XMFLOAT3 ambient;
	float pad1;
	XMFLOAT3 diffuse;
	float pad2;
	XMFLOAT3 specular;
	float alpha;
};

struct ConstBufferDataB0
{
	XMMATRIX mat;
};

struct WorldTransfom {
	// 定数バッファ(行列)
	ID3D12Resource* constBuffTransform = {};

	// 定数バッファマップ(行列)
	ConstBufferDataB0* constMapTransform = {};

	// アフィン変換
	KMyMath::Vector3 scale = { 1,1,1 };
	KMyMath::Vector3 rot = { 0,0,0 };
	KMyMath::Vector3 pos = { 0,0,0 };

	// ワールド変換行列
	XMMATRIX matWorld = {};

	// 親オブジェクトへのポインタ
	WorldTransfom* parent = nullptr;
};

class KObject3d
{
public:
	KObject3d();
	void Initialize();
	void LoadModel(KModel* model);
	void SetPipeline(KGPlin* pipeline_);
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Draw(KTexture* texture);

	void Finalize();

	static void StaticInitialize();
	static KObject3d Create();

private:
	static KGPlin* pipeline;

public:
	KMyMath::Vector3 rotResult;

	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;

	// 3Dオブジェクトの配列
	WorldTransfom transform;

	HRESULT result;

	KModel* model_ = nullptr;

	KTexture* texture_ = nullptr;
};

