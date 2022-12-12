#include "KModel.h"
#include "KDirectXCommon.h"

KModel::ObjMaterialInfo KModel::objMtl;

Cube::Cube() {
	vertices = {
		//  x	  y	    z	 n    u	   v
		// 前
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
		{{-5.0f, 5.0f,-5.0f},{},{0.0f,0.0f}}, // 左上
		{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,0.0f}}, // 右上
		// 後
		{{ 5.0f,-5.0f, 5.0f},{},{1.0f,1.0f}}, // 右下
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,1.0f}}, // 左下
		{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
		// 左
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
		{{-5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
		{{-5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
		// 右
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
		{{ 5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
		{{ 5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
		// 下
		{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
		{{ 5.0f,-5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
		// 上
		{{-5.0f, 5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
		{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}  // 右上
	};

	indices = {
		// 前
		 0, 1, 2, // 三角形1つ目
		 2, 1, 3, // 三角形2つ目
		// 後
		 4, 5, 6,
		 6, 5, 7,
		 // 左
		  8, 9,10,
		 10, 9,11,
		 // 右
		 12,13,14,
		 14,13,15,
		 // 下
		 16,17,18,
		 18,17,19,
		 // 上
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
		// 前
		{{-0.1f,-0.1f,-40.0f},{},{0.0f,1.0f}}, // 左下
		{{-0.1f, 0.1f,-40.0f},{},{0.0f,0.0f}}, // 左上
		{{ 0.1f,-0.1f,-40.0f},{},{1.0f,1.0f}}, // 右下
		{{ 0.1f, 0.1f,-40.0f},{},{1.0f,0.0f}}, // 右上
		// 後
		{{ 0.1f,-0.1f, 40.0f},{},{1.0f,1.0f}}, // 右下
		{{ 0.1f, 0.1f, 40.0f},{},{1.0f,0.0f}}, // 右上
		{{-0.1f,-0.1f, 40.0f},{},{0.0f,1.0f}}, // 左下
		{{-0.1f, 0.1f, 40.0f},{},{0.0f,0.0f}}, // 左上
		// 左
		{{-0.1f,-0.1f,-40.0f},{},{0.0f,1.0f}}, // 左下
		{{-0.1f,-0.1f, 40.0f},{},{0.0f,0.0f}}, // 左上
		{{-0.1f, 0.1f,-40.0f},{},{1.0f,1.0f}}, // 右下
		{{-0.1f, 0.1f, 40.0f},{},{1.0f,0.0f}}, // 右上
		// 右
		{{ 0.1f, 0.1f,-40.0f},{},{1.0f,1.0f}}, // 右下
		{{ 0.1f, 0.1f, 40.0f},{},{1.0f,0.0f}}, // 右上
		{{ 0.1f,-0.1f,-40.0f},{},{0.0f,1.0f}}, // 左下
		{{ 0.1f,-0.1f, 40.0f},{},{0.0f,0.0f}}, // 左上
		// 下
		{{ 0.1f,-0.1f,-40.0f},{},{1.0f,1.0f}}, // 右下
		{{ 0.1f,-0.1f, 40.0f},{},{1.0f,0.0f}}, // 右上
		{{-0.1f,-0.1f,-40.0f},{},{0.0f,1.0f}}, // 左下
		{{-0.1f,-0.1f, 40.0f},{},{0.0f,0.0f}}, // 左上
		// 上
		{{-0.1f, 0.1f,-40.0f},{},{0.0f,1.0f}}, // 左下
		{{-0.1f, 0.1f, 40.0f},{},{0.0f,0.0f}}, // 左上
		{{ 0.1f, 0.1f,-40.0f},{},{1.0f,1.0f}}, // 右下
		{{ 0.1f, 0.1f, 40.0f},{},{1.0f,0.0f}}  // 右上
	};

	indices = {
		// 前
		 0, 1, 2, // 三角形1つ目
		 2, 1, 3, // 三角形2つ目
		// 後
		 4, 5, 6,
		 6, 5, 7,
		 // 左
		  8, 9,10,
		 10, 9,11,
		 // 右
		 12,13,14,
		 14,13,15,
		 // 下
		 16,17,18,
		 18,17,19,
		 // 上
		 20,21,22,
		 22,21,23
	};
}

void KModel::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	KTexture texture;
	// ファイルストリーム
	std::ifstream file;
	// マテリアルファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}
	// 1行ずつ読み込む
	std::string line;
	while (getline(file,line))
	{
		// 1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		// 半角スペース区切りで先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');
		// 先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}
		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			// マテリアル名読み込み
			line_stream >> objMtl.name;
		}
		// 先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> objMtl.ambient.x;
			line_stream >> objMtl.ambient.y;
			line_stream >> objMtl.ambient.z;
		}
		// 先頭文字列がKdならアンビエント色
		if (key == "Kd")
		{
			line_stream >> objMtl.diffuse.x;
			line_stream >> objMtl.diffuse.y;
			line_stream >> objMtl.diffuse.z;
		}
		// 先頭文字列がKsならアンビエント色
		if (key == "Ks")
		{
			line_stream >> objMtl.specular.x;
			line_stream >> objMtl.specular.y;
			line_stream >> objMtl.specular.z;
		}
		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			// テクスチャのファイル名読み込み
			line_stream >> objMtl.textureFilename;
			// テクスチャ読み込み
			texture.LoadTexture(directoryPath, objMtl.textureFilename);
		}
	}
	// ファイルを閉じる
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
