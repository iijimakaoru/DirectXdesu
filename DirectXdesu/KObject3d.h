#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Vector3.h"
#include "KModel.h"
#include "KTexture.h"
#include "KMaterial.h"
//#include <memory>
#include "DirectionLight.h"
#include "ViewProjection.h"
#include "LightGroup.h"

using namespace DirectX;

// 定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	XMMATRIX mat; // 3D変換行列
};

struct WorldTransfom {
	// 定数バッファ(行列)
	ComPtr<ID3D12Resource> constBuffTransform = {};

	// 定数バッファマップ(行列)
	ConstBufferDataTransform* constMapTransform = {};

	// アフィン変換
	Vector3 scale = { 1,1,1 };
	Vector3 rot = { 0,0,0 };
	Vector3 pos = { 0,0,0 };

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
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Draw(KTexture* texture);

	/*static void SetLight(DirectionLight* light)
	{
		KObject3d::light = light;
	}*/

	static void SetLightGroup(LightGroup* lightGroup)
	{
		KObject3d::lightGroup = lightGroup;
	}

	static void StaticInitialize();
	static KObject3d Create();

	Vector3 rotResult;

	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;

	// 3Dオブジェクトの配列
	WorldTransfom transform;

	HRESULT result;

	KModel* model_ = nullptr;

	KTexture* texture_ = nullptr;

	// マテリアル
	KMaterial* material = nullptr;

	//static DirectionLight* light;
	static LightGroup* lightGroup;
};

