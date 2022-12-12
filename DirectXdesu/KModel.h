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
		// �}�e���A����
		std::string name;
		// �A���r�G���g�e���x
		XMFLOAT3 ambient;
		// �f�B�q���[�Y�e���x
		XMFLOAT3 diffuse;
		// �X�y�L�����[�e���x
		XMFLOAT3 specular;
		// �A���t�@
		float alpha;
		// �e�N�X�`���t�@�C����
		std::string textureFilename;
		// �R���X�g���N�^
		ObjMaterialInfo()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	// ���_�f�[�^
	std::vector<VertexPosNormalUV> vertices;
	// �C���f�b�N�X�f�[�^
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