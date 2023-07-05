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
		KMyMath::Matrix4 viewproj; // ビュープロジェクション
		KMyMath::Matrix4 world;	   // ワールド行列
		KMyMath::Vector3 cameraPos;// カメラ座標
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewProjection"></param>
	void Update(ViewProjection* viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パイプラインセッター
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipline(KGPlin* pipeline_)
	{
		FbxObject3D::pipeline = pipeline_;
	}

	/// <summary>
	/// モデルセッター
	/// </summary>
	/// <param name="model_"></param>
	void SetModel(FbxModel* model_) 
	{ 
		model = model_; 
	}

	/// <summary>
	/// ポジションセッター
	/// </summary>
	/// <param name="pos_"></param>
	void SetPosition(KMyMath::Vector3 pos_)
	{
		position = pos_;
	}

	/// <summary>
	/// ポジションゲッター
	/// </summary>
	/// <returns></returns>
	KMyMath::Vector3 GetPosition()
	{
		return position;
	}

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation();

public:// 定数
	// ボーンの最大値
	static const int MAX_BONES = 32;

	struct ConstBufferDataSkin
	{
		KMyMath::Matrix4 bones[MAX_BONES];
	};

protected:
	ComPtr<ID3D12Resource> constBuffTransform;
	HRESULT result;

	// ローカルスケール
	KMyMath::Vector3 scale = { 1,1,1 };
	// ローカル回転角
	KMyMath::Vector3 rotation = { 0,0,0 };
	// ローカル座標
	KMyMath::Vector3 position = { 0,0,0 };
	// ローカルワールド変換行列
	KMyMath::Matrix4 matWorld;
	// モデル
	FbxModel* model = nullptr;

	ComPtr<ID3D12Resource> constBuffSkin;

	// 1フレームの時間
	FbxTime frameTime;
	// アニメーション開始時間
	FbxTime startTime;
	// アニメーション終了時間
	FbxTime endTime;
	// アニメーション現在時間
	FbxTime currentTime;
	// アニメーション再生フラグ
	bool isPlay = false;

	static KGPlin* pipeline;
};

