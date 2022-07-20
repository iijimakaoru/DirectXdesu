#pragma once
#include <vector>
#include "KVertex.h"

class KModel : public KVertex {
public:
	// 頂点データ
	std::vector<Vertex> vertices;
	// インデックスデータ
	std::vector<short> indices;
	// modelusakusei
	void CreateModel(KDirectInit dx);
};

class Cube : public KModel {
public:
	Cube();
	~Cube() {};
};