#include "KModel.h"

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
	//vertices = {
	//	{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
	//	{{-5.0f, 5.0f,-5.0f},{},{0.0f,0.0f}}, // 左上
	//	{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}  // 右下
	//};

	//indices = {
	//	 0, 1, 2, // 三角形1つ目
	//};

	ifstream file;

	file.open("Resources/triangle.obj");
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

			VertexPosNormalUV vertex{};
			vertex.pos = pos;
			vertices.emplace_back(vertex);
		}

		if (key == "f")
		{
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				std::istringstream index_stream(index_string);

				unsigned short indexPosition;

				index_stream >> indexPosition;

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

void KModel::CreateModel(ID3D12Device* dev)
{
	vertexs.reset(new KVertex(dev, vertices, indices));
}