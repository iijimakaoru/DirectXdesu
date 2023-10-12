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

class KModel 
{
public:
	struct ObjMaterialInfo
	{
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

	// 頂点データ
	std::vector<VertexPosNormalUV> vertices;
	// インデックスデータ
	std::vector<unsigned short> indices;

	std::unique_ptr<KVertex> vertexs;

	TextureData texData;

	ObjMaterialInfo objMtl;

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	// modelusakusei
	void CreateModel();

	void Draw();
	void Draw(TextureData texData);
};

class Cube : public KModel 
{
public:
	Cube();
	~Cube();
};

class Triangle : public KModel 
{
public:
	Triangle();
	~Triangle();
};

class Line : public KModel 
{
public:
	Line();
	~Line();
};

class MtlObj : public KModel
{
public:
	MtlObj(const std::string modelname);
	~MtlObj();
};