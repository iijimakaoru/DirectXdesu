#pragma once
#include "FbxModel.h"
#include "Camera.h"
#include "KGPlin.h"

class FbxObject3D
{
protected: // エイリアス
	// 省略処理
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	// セッター

public: // エイリアス
	// 定数バッファデータ構造体
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj; // ビュープロジェクション
		XMMATRIX world;	   // ワールド行列
		XMFLOAT3 cameraPos;// カメラ座標
	};

public:
	void Init();

	void Update(ViewProjection& viewProjection);

	void Draw();

	void SetPipline(KGPlin* pipeline_)
	{
		FbxObject3D::pipeline = pipeline_;
	}

	void SetModel(FbxModel* model_) 
	{ 
		model = model_; 
	}

	void SetPosition(XMFLOAT3 pos_)
	{
		position = pos_;
	}

	XMFLOAT3 GetPosition()
	{
		return position;
	}

protected:
	ComPtr<ID3D12Resource> constBuffTransform;
	HRESULT result;

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// ローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// モデル
	FbxModel* model = nullptr;

	static KGPlin* pipeline;
};

