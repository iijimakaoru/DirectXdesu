#pragma once
#include <vector>
#include "KVertex.h"

class KModel : public KVertex{
public:
	// ���_�f�[�^
	std::vector<Vertex> vertices;
	// �C���f�b�N�X�f�[�^
	std::vector<short> indices;
	// modelusakusei
	void CreateModel(ID3D12Device* dev);
};

class Cube : public KModel{
public:
	Cube();
	~Cube(){};
};