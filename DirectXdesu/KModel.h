#pragma once
#include <vector>
#include "KVertex.h"

class KModel 
{
public:
	// ���_�f�[�^
	static std::vector<Vertex> vertices;
	// �C���f�b�N�X�f�[�^
	static std::vector<short> indices;
};

class Cube : public KModel
{
public:
	Cube();
	~Cube(){};

};