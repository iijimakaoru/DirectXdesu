#pragma once
#include <vector>
#include "KVertex.h"
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <DirectXTex.h>
#include "KTexture.h"

using namespace DirectX;
using namespace std;

class KModel {
public:
	struct ObjMaterialInfo
	{
		// マテリアル名
		std::string name;
		// アンビエント影響度
		XMFLOAT3 ambient;
		// ディヒューズ影響度
		XMFLOAT3 diffuse;
		// スペキュラー影響度
		XMFLOAT3 specular;
		// アルファ
		float alpha;
		// テクスチャファイル名
		std::string textureFilename;
		// コンストラクタ
		ObjMaterialInfo()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	// 頂点データ
	std::vector<VertexPosNormalUV> vertices;
	// インデックスデータ
	std::vector<unsigned short> indices;

	std::unique_ptr<KVertex> vertexs = make_unique<KVertex>();

	

	static ObjMaterialInfo objMtl;

	static void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	// modelusakusei
	void CreateModel();
};

class Cube : public KModel {
public:
	Cube();
	~Cube() {};
};

class Triangle : public KModel {
public:
	Triangle();
	~Triangle(){}
};

class Line : public KModel {
public:
	Line();
	~Line(){}
};

class MtlObj : public KModel
{
public:
	MtlObj();
	~MtlObj() {};
};