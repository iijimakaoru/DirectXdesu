#pragma once
#include "KVertex.h"
#include <fstream>
#include <sstream>
#include <string>
#include "TextureManager.h"

class KModel 
{
public:
	struct ObjMaterialInfo
	{
		// �}�e���A����
		std::string name;
		// �A���r�G���g�e���x
		KMyMath::Vector3 ambient;
		// �f�B�q���[�Y�e���x
		KMyMath::Vector3 diffuse;
		// �X�y�L�����[�e���x
		KMyMath::Vector3 specular;
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

	std::unique_ptr<KVertex> vertexs =std::make_unique<KVertex>();

	TextureData texData;

	ObjMaterialInfo objMtl;

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	// modelusakusei
	void CreateModel();

	void Draw();
	void Draw(TextureData texData);
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
	MtlObj(const std::string modelname);
	~MtlObj() {};
};