#pragma once
#include <vector>
#include "KVertex.h"

class Model : public KVertex {
public:
	// ���_�f�[�^
	std::vector<Vertex> vertices;
	// �C���f�b�N�X�f�[�^
	std::vector<short> indices;
	// modelusakusei
	void CreateModel(KDirectInit dx);
};

class Cube : public Model {
public:
	Cube();
	~Cube() {};
};