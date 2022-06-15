#pragma once
#include <vector>
#include "KVertex.h"

class KModel 
{
public:
	// 頂点データ
	static std::vector<Vertex> vertices;
	// インデックスデータ
	static std::vector<short> indices;
};

class Cube : public KModel
{
public:
	Cube();
	~Cube(){};

};