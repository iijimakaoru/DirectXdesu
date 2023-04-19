#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "KVertexBuff.h"
#include "KIndexBuff.h"
#include "KComputePipelineState.h"
#include "KShader.h"

struct KTextureData
{
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	//デスクプリタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> srvHeap;

	//GPUデスクプリタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

	//横幅
	size_t width = 0;
	//縦幅
	size_t height = 0;

	//カラー
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	std::string path;

	uint32_t textureHandle;

	~KTextureData();

private:
	char PADING[4]{};
};

// 座標変換行列
struct KConstBufferDataTransform
{
	Matrix mat; // 3D変換行列
};

struct KPosColor
{
	Vector3 pos; // 座標
	Vector4 color; // カラー
};

struct KPosNormalUV
{
	Vector3 pos; // 座標
	Vector3 normal; // 法線ベクトル
	Vector2 uv; // uv座標
};

// 座標・UV・カラー
struct KPosUvColor
{
	Vector3 pos; // 座標
	Vector2 uv; // uv座標
	Vector4 color; // カラー
};

typedef struct
{
	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	// スプライト数
	UINT nextIndex;
}KModelShareVaria;

struct KPipelineSet
{
	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

struct KworldViewpojCamera
{
	//ワールド行列
	Matrix world;
	//ワールド座標
	Matrix matWorld;
	//カメラ座標(ワールド座標)
	Vector3 cameraPos;
};

struct worldViewpojCameraWVP
{
	//ワールド行列
	Matrix world;
	//ワールド座標
	Matrix matWorld;
	//カメラ座標(ワールド座標)
	Vector3 cameraPos;
	Matrix WVP;
};

//マテリアルよう定数バッファ構造体
struct KConstBuffDataMaterial
{
	//アンビエント係数
	Vector3 ambient;
	//パディング
	float pad1;
	//ディフューズ係数
	Vector3 diffuse;
	//パディング
	float pad2;
	//スペキュラー係数
	Vector3 specular;
	//アルファ
	float alpha;
};

//マテリアル
struct KModelMaterial
{
	//マテリアル名
	std::string name;
	//アンビエント影響度
	Vector3 ambient = { 0.06f, 0.06f, 0.06f };
	//ディフューズ影響度
	Vector3 diffuse = { 0.0f,0.0f,0.0f };
	//スペキュラー影響度
	Vector3 specular = { 0.0f,0.0f,0.0f };

	Vector3 emission = { 0.0f,0.0f,0.0f };

	float shininess = 1.0f;
	//アルファ
	float alpha = 1.0f;
	//テクスチャファイル名
	std::string textureFiename;

	KConstBuffDataMaterial GetConstBuffMaterial();
};

//バッファ関連
struct KBuff
{
	//頂点バッファ
	std::unique_ptr<KVertexBuff> vertexBuffer;
	//頂点マップ
	KPosColor* vertMap;
	//インデックスバッファ
	std::unique_ptr<KIndexBuff> indexBuffer;
	//インデックスマップ
	uint32_t* indexMap;

	KBuff() = default;
	~KBuff() = default;

private:
	//コピーコンストラクタ・代入演算子削除
	KBuff& operator=(const KBuff&) = delete;
	KBuff(const KBuff&) = delete;

};

//ブレンドモード
enum class BlendMode
{
	AX_BLENDMODE_NOBLEND,//ノーブレンド（デフォルト）
	AX_BLENDMODE_ALPHA,//αブレンド
	AX_BLENDMODE_ADD,//加算ブレンド
	AX_BLENDMODE_SUB,//減算ブレンド
	AX_BLENDMODE_MULA,//乗算ブレンド
	AX_BLENDMODE_INVSRC,//反転ブレンド

	AX_BLENDMODE_MAX,//ブレンド種類数

	AX_BLENDMODE_CUSTOM = -1,
};

//コンピュートシェーダー関連(クラス共通)
struct ComputeRelation
{
	ComputeRelation();
	~ComputeRelation();

	//パイプラインステート
	std::shared_ptr<KComputePipelineState> computePipelineState;
	//ルートシグネチャ
	std::unique_ptr<KRootSignature> rootSignature;

	std::unique_ptr<KRootSignature> rootSignature1;
	//シェーダー
	std::unique_ptr<KShader> computeShader;

private:
	//コピーコンストラクタ・代入演算子削除
	ComputeRelation& operator=(const ComputeRelation&) = delete;
	ComputeRelation(const ComputeRelation&) = delete;
};