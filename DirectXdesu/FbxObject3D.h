#pragma once
#include "FbxModel.h"
#include "Camera.h"

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

protected:
	ComPtr<ID3D12Resource> constBuffTransform;
	HRESULT result;
};

