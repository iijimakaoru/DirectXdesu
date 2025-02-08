#pragma once
#include "KVertex.h"
#include "TextureManager.h"
#include <fstream>
#include <sstream>
#include <string>

/**
 * @file KModel.h
 * @brief モデル
 * @author 飯島 薫
 */

class KModel {
public:
	struct ObjMaterialInfo {
		// マテリアル名
		std::string name;
		// アンビエント影響度
		KMyMath::Vector3 ambient;
		// ディヒューズ影響度
		KMyMath::Vector3 diffuse;
		// スペキュラー影響度
		KMyMath::Vector3 specular;
		// アルファ
		float alpha;
		// テクスチャファイル名
		std::string textureFilename;
		// コンストラクタ
		ObjMaterialInfo();
	};

	struct ConstBufferDataB1 {
		KMyMath::Vector3 ambient;
		float pad1;
		KMyMath::Vector3 diffuse;
		float pad2;
		KMyMath::Vector3 specular;
		float alpha;
	};

public:
	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void Init();

	void CreateConstBuffer();

	void Update();

	// 描画
	void Draw(UINT rootParamIndex);

	// 描画(テクスチャ指定)
	void Draw(UINT rootParamIndex, const TextureData& texData);

	// モデル生成
	void CreateModel();

public:
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;

	// 頂点データ
	std::vector<VertexPosNormalUV> vertices;
	// インデックスデータ
	std::vector<unsigned short> indices;

	std::unique_ptr<KVertex> vertexs;

	TextureData texData;

	ObjMaterialInfo objMtl;

	std::unordered_map<uint16_t, std::vector<uint16_t>>smoothData;
private:
};

class Cube : public KModel {
public:
	Cube();
	~Cube();
};

class Triangle : public KModel {
public:
	Triangle();
	~Triangle();
};

class Line : public KModel {
public:
	Line();
	~Line();
};

class MtlObj : public KModel {
public:
	MtlObj(const std::string& modelname,bool smoothing = false);
	~MtlObj();

	void CalculateSmoothedVertexNormals();
};

class MeshModel {
private:
	std::vector<Vertex> vertices;
	std::unique_ptr<KVertex> vertexs;

public:
	MeshModel(const std::string& modelname);
	~MeshModel();

	std::vector<Vertex> GetVertices();
	KVertex* GetVertex();
};