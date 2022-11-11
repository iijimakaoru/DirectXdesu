#pragma once
#include <vector>
#include "KVertex.h"
#include <memory>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class KModel {
public:
	// 頂点データ
	std::vector<VertexPosNormalUV> vertices;
	// インデックスデータ
	std::vector<unsigned short> indices;

	std::unique_ptr<KVertex> vertexs = make_unique<KVertex>();

	// modelusakusei
	void CreateModel(ID3D12Device* dev);
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