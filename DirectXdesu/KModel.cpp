#include "KModel.h"
#include "KDirectXCommon.h"

KModel::ObjMaterialInfo KModel::objMtl;

Cube::Cube() {
	vertices = {
		//  x	  y	    z	 n    u	   v
		// �O
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // ����
		{{-5.0f, 5.0f,-5.0f},{},{0.0f,0.0f}}, // ����
		{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}, // �E��
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,0.0f}}, // �E��
		// ��
		{{ 5.0f,-5.0f, 5.0f},{},{1.0f,1.0f}}, // �E��
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // �E��
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,1.0f}}, // ����
		{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}}, // ����
		// ��
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // ����
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // ����
		{{-5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // �E��
		{{-5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // �E��
		// �E
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // �E��
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // �E��
		{{ 5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // ����
		{{ 5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // ����
		// ��
		{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}, // �E��
		{{ 5.0f,-5.0f, 5.0f},{},{1.0f,0.0f}}, // �E��
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // ����
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // ����
		// ��
		{{-5.0f, 5.0f,-5.0f},{},{0.0f,1.0f}}, // ����
		{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}}, // ����
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // �E��
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}  // �E��
	};

	indices = {
		// �O
		 0, 1, 2, // �O�p�`1��
		 2, 1, 3, // �O�p�`2��
		// ��
		 4, 5, 6,
		 6, 5, 7,
		 // ��
		  8, 9,10,
		 10, 9,11,
		 // �E
		 12,13,14,
		 14,13,15,
		 // ��
		 16,17,18,
		 18,17,19,
		 // ��
		 20,21,22,
		 22,21,23
	};
}

Triangle::Triangle() {
	ifstream file;

	file.open("Resources/obj/triangle_tex.obj");

	assert(!file.fail());

	vector<XMFLOAT3> positions;
	vector<XMFLOAT3> normals;
	vector<XMFLOAT2> texcoords;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUV) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	vertexs->vbView.SizeInBytes = sizeVB;
	vertexs->ibView.SizeInBytes = sizeIB;

	std::copy(vertices.begin(), vertices.end(), vertexs->vertMap);

	std::copy(indices.begin(), indices.end(), vertexs->indexMap);

	std::string line;
	while (getline(file, line))
	{

		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "v")
		{
			XMFLOAT3 pos{};
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;

			positions.emplace_back(pos);
		}

		if (key == "vt")
		{
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			texcoord.y = 1.0f - texcoord.y;

			texcoords.emplace_back(texcoord);
		}

		if (key == "vn")
		{
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			normals.emplace_back(normal);
		}

		if (key == "f")
		{
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				std::istringstream index_stream(index_string);

				unsigned short indexPosition, indexNormal, indexTexcoord;

				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				VertexPosNormalUV vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				indices.emplace_back(indexPosition - 1);
			}
		}
	}
	file.close();
}

Line::Line() {
	vertices = {
		// �O
		{{-0.1f,-0.1f,-40.0f},{},{0.0f,1.0f}}, // ����
		{{-0.1f, 0.1f,-40.0f},{},{0.0f,0.0f}}, // ����
		{{ 0.1f,-0.1f,-40.0f},{},{1.0f,1.0f}}, // �E��
		{{ 0.1f, 0.1f,-40.0f},{},{1.0f,0.0f}}, // �E��
		// ��
		{{ 0.1f,-0.1f, 40.0f},{},{1.0f,1.0f}}, // �E��
		{{ 0.1f, 0.1f, 40.0f},{},{1.0f,0.0f}}, // �E��
		{{-0.1f,-0.1f, 40.0f},{},{0.0f,1.0f}}, // ����
		{{-0.1f, 0.1f, 40.0f},{},{0.0f,0.0f}}, // ����
		// ��
		{{-0.1f,-0.1f,-40.0f},{},{0.0f,1.0f}}, // ����
		{{-0.1f,-0.1f, 40.0f},{},{0.0f,0.0f}}, // ����
		{{-0.1f, 0.1f,-40.0f},{},{1.0f,1.0f}}, // �E��
		{{-0.1f, 0.1f, 40.0f},{},{1.0f,0.0f}}, // �E��
		// �E
		{{ 0.1f, 0.1f,-40.0f},{},{1.0f,1.0f}}, // �E��
		{{ 0.1f, 0.1f, 40.0f},{},{1.0f,0.0f}}, // �E��
		{{ 0.1f,-0.1f,-40.0f},{},{0.0f,1.0f}}, // ����
		{{ 0.1f,-0.1f, 40.0f},{},{0.0f,0.0f}}, // ����
		// ��
		{{ 0.1f,-0.1f,-40.0f},{},{1.0f,1.0f}}, // �E��
		{{ 0.1f,-0.1f, 40.0f},{},{1.0f,0.0f}}, // �E��
		{{-0.1f,-0.1f,-40.0f},{},{0.0f,1.0f}}, // ����
		{{-0.1f,-0.1f, 40.0f},{},{0.0f,0.0f}}, // ����
		// ��
		{{-0.1f, 0.1f,-40.0f},{},{0.0f,1.0f}}, // ����
		{{-0.1f, 0.1f, 40.0f},{},{0.0f,0.0f}}, // ����
		{{ 0.1f, 0.1f,-40.0f},{},{1.0f,1.0f}}, // �E��
		{{ 0.1f, 0.1f, 40.0f},{},{1.0f,0.0f}}  // �E��
	};

	indices = {
		// �O
		 0, 1, 2, // �O�p�`1��
		 2, 1, 3, // �O�p�`2��
		// ��
		 4, 5, 6,
		 6, 5, 7,
		 // ��
		  8, 9,10,
		 10, 9,11,
		 // �E
		 12,13,14,
		 14,13,15,
		 // ��
		 16,17,18,
		 18,17,19,
		 // ��
		 20,21,22,
		 22,21,23
	};
}

void KModel::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	KTexture texture;
	// �t�@�C���X�g���[��
	std::ifstream file;
	// �}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}
	// 1�s���ǂݍ���
	std::string line;
	while (getline(file,line))
	{
		// 1�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);
		// ���p�X�y�[�X��؂�Ő擪��������擾
		std::string key;
		getline(line_stream, key, ' ');
		// �擪�̃^�u�����͖�������
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}
		// �擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl")
		{
			// �}�e���A�����ǂݍ���
			line_stream >> objMtl.name;
		}
		// �擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			line_stream >> objMtl.ambient.x;
			line_stream >> objMtl.ambient.y;
			line_stream >> objMtl.ambient.z;
		}
		// �擪������Kd�Ȃ�A���r�G���g�F
		if (key == "Kd")
		{
			line_stream >> objMtl.diffuse.x;
			line_stream >> objMtl.diffuse.y;
			line_stream >> objMtl.diffuse.z;
		}
		// �擪������Ks�Ȃ�A���r�G���g�F
		if (key == "Ks")
		{
			line_stream >> objMtl.specular.x;
			line_stream >> objMtl.specular.y;
			line_stream >> objMtl.specular.z;
		}
		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> objMtl.textureFilename;
			// �e�N�X�`���ǂݍ���
			texture.LoadTexture(directoryPath, objMtl.textureFilename);
		}
	}
	// �t�@�C�������
	file.close();
}

void KModel::CreateModel()
{
	vertexs.reset(new KVertex(KDirectXCommon::GetInstance()->GetDev(), vertices, indices));
}

MtlObj::MtlObj()
{
	ifstream file;

	//file.open("Resources/obj/triangle_tex.obj");
	const string modelname = "triangle_mat";
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/obj/";
	file.open(directoryPath + filename);

	assert(!file.fail());

	vector<XMFLOAT3> positions;
	vector<XMFLOAT3> normals;
	vector<XMFLOAT2> texcoords;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUV) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	vertexs->vbView.SizeInBytes = sizeVB;
	vertexs->ibView.SizeInBytes = sizeIB;

	std::copy(vertices.begin(), vertices.end(), vertexs->vertMap);

	std::copy(indices.begin(), indices.end(), vertexs->indexMap);

	std::string line;
	while (getline(file, line))
	{

		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{
			// 
			string filename;
			line_stream >> filename;
			// 
			LoadMaterial(directoryPath, filename);
		}

		if (key == "v")
		{
			XMFLOAT3 pos{};
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;

			positions.emplace_back(pos);
		}

		if (key == "vt")
		{
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			texcoord.y = 1.0f - texcoord.y;

			texcoords.emplace_back(texcoord);
		}

		if (key == "vn")
		{
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			normals.emplace_back(normal);
		}

		if (key == "f")
		{
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				std::istringstream index_stream(index_string);

				unsigned short indexPosition, indexNormal, indexTexcoord;

				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				VertexPosNormalUV vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				indices.emplace_back(indexPosition - 1);
			}
		}
	}
	file.close();
}
