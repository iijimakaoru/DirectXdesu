#pragma once
#include <vector>
#include "KVertex.h"
#include <memory>

class KModel {
public:
	// ���_�f�[�^
	std::vector<Vertex> vertices;
	// �C���f�b�N�X�f�[�^
	std::vector<short> indices;

	std::unique_ptr<KVertex> vertex;

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