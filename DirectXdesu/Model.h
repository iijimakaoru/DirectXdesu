#pragma once
#include <vector>
#include "KVertex.h"

class Model : public KVertex {
public:
	// 頂点データ
	std::vector<Vertex> vertices;
	// インデックスデータ
	std::vector<short> indices;
	// modelusakusei
	void CreateModel(KDirectInit dx);
};

class Cube : public Model {
public:
	Cube();
	~Cube() {};
};