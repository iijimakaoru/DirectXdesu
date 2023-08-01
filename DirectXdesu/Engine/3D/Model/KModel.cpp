#include "KModel.h"
#include "KDirectXCommon.h"

Cube::Cube() {
	vertices = {
		//  x	  y	    z	 n    u	   v
		// 前
		{{-1.0f,-1.0f,-1.0f},{},{0.0f,1.0f}}, // 左下
		{{-1.0f, 1.0f,-1.0f},{},{0.0f,0.0f}}, // 左上
		{{ 1.0f,-1.0f,-1.0f},{},{1.0f,1.0f}}, // 右下
		{{ 1.0f, 1.0f,-1.0f},{},{1.0f,0.0f}}, // 右上
		// 後
		{{ 1.0f,-1.0f, 1.0f},{},{1.0f,1.0f}}, // 右下
		{{ 1.0f, 1.0f, 1.0f},{},{1.0f,0.0f}}, // 右上
		{{-1.0f,-1.0f, 1.0f},{},{0.0f,1.0f}}, // 左下
		{{-1.0f, 1.0f, 1.0f},{},{0.0f,0.0f}}, // 左上
		// 左
		{{-1.0f,-1.0f,-1.0f},{},{0.0f,1.0f}}, // 左下
		{{-1.0f,-1.0f, 1.0f},{},{0.0f,0.0f}}, // 左上
		{{-1.0f, 1.0f,-1.0f},{},{1.0f,1.0f}}, // 右下
		{{-1.0f, 1.0f, 1.0f},{},{1.0f,0.0f}}, // 右上
		// 右
		{{ 1.0f, 1.0f,-1.0f},{},{1.0f,1.0f}}, // 右下
		{{ 1.0f, 1.0f, 1.0f},{},{1.0f,0.0f}}, // 右上
		{{ 1.0f,-1.0f,-1.0f},{},{0.0f,1.0f}}, // 左下
		{{ 1.0f,-1.0f, 1.0f},{},{0.0f,0.0f}}, // 左上
		// 下
		{{ 1.0f,-1.0f,-1.0f},{},{1.0f,1.0f}}, // 右下
		{{ 1.0f,-1.0f, 1.0f},{},{1.0f,0.0f}}, // 右上
		{{-1.0f,-1.0f,-1.0f},{},{0.0f,1.0f}}, // 左下
		{{-1.0f,-1.0f, 1.0f},{},{0.0f,0.0f}}, // 左上
		// 上
		{{-1.0f, 1.0f,-1.0f},{},{0.0f,1.0f}}, // 左下
		{{-1.0f, 1.0f, 1.0f},{},{0.0f,0.0f}}, // 左上
		{{ 1.0f, 1.0f,-1.0f},{},{1.0f,1.0f}}, // 右下
		{{ 1.0f, 1.0f, 1.0f},{},{1.0f,0.0f}}  // 右上
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

Triangle::Triangle() 
{
	ifstream file;

	file.open("Resources/obj/triangle_tex.obj");

	assert(!file.fail());

	vector<KMyMath::Vector3> positions;
	vector<KMyMath::Vector3> normals;
	vector<KMyMath::Vector2> texcoords;

	std::string line;
	while (getline(file, line))
	{

		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "v")
		{
			KMyMath::Vector3 pos{};
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;

			positions.emplace_back(pos);
		}

		if (key == "vt")
		{
			KMyMath::Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			texcoord.y = 1.0f - texcoord.y;

			texcoords.emplace_back(texcoord);
		}

		if (key == "vn")
		{
			KMyMath::Vector3 normal{};
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
			texData = TextureManager::Load(directoryPath + objMtl.textureFilename);
		}
	}
	// ファイルを閉じる
	file.close();
}

void KModel::CreateModel()
{
	vertexs.reset(new KVertex(KDirectXCommon::GetInstance()->GetDev(), vertices, indices));
}

void KModel::Draw()
{
	// 頂点バッファビューの設定
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetVertexBuffers(0, 1, &vertexs->vbView);

	// インデックスバッファビューの設定
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetIndexBuffer(&vertexs->ibView);

	// デスクリプタヒープのセット
	ID3D12DescriptorHeap* ppHeaps[] = { texData.srvHeap.Get() };
	KDirectXCommon::GetInstance()->GetCmdlist()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// シェーダーリソースビューをセット
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootDescriptorTable(0, texData.gpuHandle);

	// 描画
	KDirectXCommon::GetInstance()->GetCmdlist()->DrawIndexedInstanced(static_cast<UINT>(indices.size()),
		static_cast < UINT>(1), static_cast < UINT>(0), static_cast < UINT>(0), static_cast < UINT>(0));
}

void KModel::Draw(TextureData texData)
{
	// 頂点バッファビューの設定
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetVertexBuffers(0, 1, &vertexs->vbView);

	// インデックスバッファビューの設定
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetIndexBuffer(&vertexs->ibView);

	// デスクリプタヒープのセット
	ID3D12DescriptorHeap* ppHeaps[] = { texData.srvHeap.Get() };
	KDirectXCommon::GetInstance()->GetCmdlist()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texData.gpuHandle;

	// シェーダーリソースビューをセット
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootDescriptorTable(0, srvGpuHandle);

	// 描画
	KDirectXCommon::GetInstance()->GetCmdlist()->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
}

MtlObj::MtlObj(const string modelname)
{
	ifstream file;

	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/obj/" + modelname + "/";
	file.open(directoryPath + filename);

	assert(!file.fail());

	vector<KMyMath::Vector3> positions;
	vector<KMyMath::Vector3> normals;
	vector<KMyMath::Vector2> texcoords;

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
			KMyMath::Vector3 pos{};
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;

			positions.emplace_back(pos);
		}

		if (key == "vt")
		{
			KMyMath::Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			texcoord.y = 1.0f - texcoord.y;

			texcoords.emplace_back(texcoord);
		}

		if (key == "vn")
		{
			KMyMath::Vector3 normal{};
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

				indices.emplace_back((unsigned short)indices.size());
			}
		}
	}
	file.close();
}
