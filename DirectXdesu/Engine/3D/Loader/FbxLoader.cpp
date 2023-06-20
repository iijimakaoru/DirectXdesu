#include "FbxLoader.h"
#include <cassert>

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const std::string FbxLoader::baseDirectory = "Resources/";
const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Init()
{
	// 再初期化チェック
	assert(fbxManager == nullptr);
	// Fbxマネージャーの生成
	fbxManager = FbxManager::Create();
	// Fbxマネージャーの入出力設定
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);
	// Fbxインポーターの生成
	fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize()
{
	// インスタンスの破棄
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

FbxModel* FbxLoader::LoadModelFromFile(const string& modelName)
{
	// 格納場所+モデル名
	const string directoryPath = baseDirectory + modelName + "/";
	// 拡張子、fbxを追加
	const string fileName = modelName + ".fbx";
	// 連結してフルパスを得る
	const string fullPath = directoryPath + fileName;
	// ファイル名を指定して読み込む
	if (!fbxImporter->Initialize(fullPath.c_str(), -1, fbxManager->GetIOSettings()))
	{
		assert(0);
	}
	// シーン生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	// ファイルから読み込んだfbx情報をシーンにインポート
	fbxImporter->Import(fbxScene);
	// モデル作成
	FbxModel* fbxModel = new FbxModel();
	fbxModel->name = modelName;
	// Fbxノードの数を取得
	int nodeCount = fbxScene->GetNodeCount();
	// あらかじめ必要分のメモリを確保することで、アドレスがずれるのを予防
	fbxModel->nodes.reserve(nodeCount);
	// ルートノードから順に解析してモデルに流し込む
	ParseNodeRecursive(fbxModel, fbxScene->GetRootNode());
	// Fbxシーン解放
	fbxModel->fbxScene = fbxScene;

	fbxModel->CreateBuffer();

	return fbxModel;
}

void FbxLoader::ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent)
{
	// ノード名を取得
	string name = fbxNode->GetName();
	// モデルにノードを追加
	model->nodes.emplace_back();
	Node& node = model->nodes.back();
	// Fbxノードの情報を解析してノードに記録
	node.name = fbxNode->GetName();
	// Fbxノードのメッシュ情報を解析
	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();
	// 形式変換して代入
	node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
	node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
	node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],1.0f };
	// 回転角をDegree(度)からラジアンに変換
	node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
	node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
	node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);
	// スケール、回転、平行移動行列の計算
	XMMATRIX matScaling, matRotation, matTransition;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTransition = XMMatrixTranslationFromVector(node.translation);
	// ローカル変形行列の計算
	node.transform = XMMatrixIdentity();
	node.transform *= matScaling; // スケーリング
	node.transform *= matRotation; // 回転
	node.transform *= matTransition; // 平行移動

	// グローバル変形行列の計算
	node.globalTransform = node.transform;
	if (parent)
	{
		node.parent = parent;
		// 親の変形を計算
		node.globalTransform *= parent->globalTransform;
	}

	// Fbxノードのメッシュ情報を解析
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

	if (fbxNodeAttribute)
	{
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			model->meshNode = &node;
			ParseMesh(model, fbxNode);
		}
	}

	// 子ノードに対して再帰呼び出し
	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
	}
}

void FbxLoader::ParseMesh(FbxModel* model, FbxNode* fbxNode)
{
	// ノードのメッシュを取得
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	// 頂点座標読み取り
	ParseMeshVertices(model, fbxMesh);
	// 面を構成するデータの読み取り
	ParseMeshFaces(model, fbxMesh);
	// マテリアル読み込み
	ParseMaterial(model, fbxNode);
	// スキニング
	ParseSkin(model, fbxMesh);
}

void FbxLoader::ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices;

	// 頂点座標データの数
	const int controlPointsCount = fbxMesh->GetControlPointsCount();

	// 必要数だけ頂点データ配列を確保
	FbxModel::VertexPosNormalUVSkin vert{};
	model->vertices.resize(controlPointsCount, vert);

	// Fbxメッシュの頂点座標配列を取得
	FbxVector4* pCoord = fbxMesh->GetControlPoints();

	// Fbxメッシュの全頂点座標をモデル内の配列にコピーする
	for (int i = 0; i < controlPointsCount; i++)
	{
		FbxModel::VertexPosNormalUVSkin& vertex = vertices[i];
		// 座標のコピー
		vertex.pos.x = static_cast<float>(pCoord[i][0]);
		vertex.pos.y = static_cast<float>(pCoord[i][1]);
		vertex.pos.z = static_cast<float>(pCoord[i][2]);
	}
}

void FbxLoader::ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices;
	auto& indices = model->indices;

	// 1ファイルに複数メッシュのモデルは非対応
	assert(indices.size() == 0);
	// 面の数
	const int polygonCount = fbxMesh->GetPolygonCount();
	// UVデータの数
	const int textureUVConut = fbxMesh->GetTextureUVCount();
	// UV名リスト
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	// 面ごとの情報読み取り
	for (int i = 0; i < polygonCount; i++)
	{
		// 面を構築する頂点の数を取得
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		// 頂点ごとの処理
		for (int j = 0; j < polygonSize; j++)
		{
			// Fbx頂点配列のインデックス
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			// 頂点法線読み込み
			FbxModel::VertexPosNormalUVSkin& vertex = vertices[index];
			FbxVector4 normal;
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertex.normal.x = static_cast<float>(normal[0]);
				vertex.normal.y = static_cast<float>(normal[1]);
				vertex.normal.z = static_cast<float>(normal[2]);
			}

			// テクスチャUV読み込み
			if (textureUVConut > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;
				// 0番決め打ちで読み込み
				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
				{
					vertex.uv.x = static_cast<float>(uvs[0]);
					vertex.uv.y = static_cast<float>(uvs[1]);
				}
			}

			// インデックス配列に頂点インデックス追加
			// 3頂点目までなら
			if (j < 3)
			{
				// 1頂点追加し、他の2点と三角形を構築する
				indices.push_back(index);
			}
			// 4頂点目
			else
			{
				// 3点追加し、
				// 四角形の0,1,2,3の内2,3,0で三角形構築する
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}
	}
}

void FbxLoader::ParseMaterial(FbxModel* model, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();
	if (materialCount > 0)
	{
		// 先頭のマテリアル取得
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);
		// テクスチャを読み込んだかどうかフラグ
		bool textureLoaded = false;

		if (material)
		{
			// FbxSurfaceLambertクラスかどうか調べる
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);
				// 環境光係数
				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				model->ambient.x = static_cast<float>(ambient.Get()[0]);
				model->ambient.y = static_cast<float>(ambient.Get()[1]);
				model->ambient.z = static_cast<float>(ambient.Get()[2]);

				// 拡散反射光係数
				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				model->diffuse.x = static_cast<float>(diffuse.Get()[0]);
				model->diffuse.y = static_cast<float>(diffuse.Get()[1]);
				model->diffuse.z = static_cast<float>(diffuse.Get()[2]);
			}

			// ディフューズテクスチャを取り出す
			const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (diffuseProperty.IsValid())
			{
				const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
				if (texture)
				{
					const char* filepath = texture->GetFileName();
					// ファイルパスからファイル名抽出
					string path_str(filepath);
					string name = ExtractFileName(path_str);
					// テクスチャ読み込み
					LoadTexture(model, baseDirectory + model->name + "/" + name);
					textureLoaded = true;
				}
			}
		}

		if (!textureLoaded)
		{
			LoadTexture(model, baseDirectory + defaultTextureFileName);
		}
	}
}

void FbxLoader::LoadTexture(FbxModel* model, const std::string& fullpath)
{
	HRESULT result = S_FALSE;
	// WICテクスチャロード
	TexMetadata& metadata = model->metadata;
	ScratchImage& scratchImage = model->scratchImg;
	// ユニコード文字列に変換
	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImage);
	if (FAILED(result))
	{
		assert(0);
	}
}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
	size_t pos1;
	// 区切り文字'\\'が出てくる一番最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	// 区切り文字'/'が出てくる一番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}

void FbxLoader::ParseSkin(FbxModel* model_, FbxMesh* fbxMesh)
{
	// スキニング情報
	FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
	// スキニング情報がないなら終了
	if (fbxSkin == nullptr)
	{
		// 各頂点についての処理
		for (int i = 0; i < model_->vertices.size(); i++)
		{
			// 最初のボーン(単位行列)の影響100%にする
			model_->vertices[i].boneIndex[0] = 0;
			model_->vertices[i].boneWeight[0] = 1.0f;
		}
		return;
	}

	// ボーンの配列の参照
	std::vector<FbxModel::Bone>& bones = model_->bones;

	// ボーンの数
	int clusterCount = fbxSkin->GetClusterCount();
	bones.reserve(clusterCount);

	// 全てのボーンについて
	for (int i = 0; i < clusterCount; i++)
	{
		// FBXボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		// ボーン自体のノードの名前を取得
		const char* boneName = fbxCluster->GetLink()->GetName();

		// 新しいボーンを追加し、追加したボーンの参照を得る
		bones.emplace_back(FbxModel::Bone(boneName));
		FbxModel::Bone& bone = bones.back();

		// 自作ボーンとFbxのボーンを紐づける
		bone.fbxCluster = fbxCluster;

		// Fbxから初期姿勢行列を取得する
		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		// XMMatrix型に変換する
		XMMATRIX initialPose;
		ConvertMatrixFromFbx(&initialPose, fbxMat);

		// 初期姿勢行列の逆行列を得る
		bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
	}

	// ボーン番号とスキンウェイトのペア
	struct WeightSet
	{
		UINT index;
		float weight;
	};

	// 二次元配列
	// list:頂点が影響を受けるボーンの全リスト
	// vector:それを全頂点分
	std::vector<std::list<WeightSet>>weightLists(model_->vertices.size());

	// 全てのボーンについて
	for (int i = 0; i < clusterCount; i++)
	{
		// FBXボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		// このボーンに影響を受ける頂点の数
		int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();

		// このボーンに影響を受ける頂点の配列
		int* controlPointIndices = fbxCluster->GetControlPointIndices();
		double* controlPointWeights = fbxCluster->GetControlPointWeights();

		// 影響を受ける全頂点について
		for (int j = 0; j < controlPointIndicesCount; j++)
		{
			// 頂点番号
			int vertIndex = controlPointIndices[j];

			// スキンウェイト
			float weight = static_cast<float>(controlPointWeights[j]);

			// その頂点の影響を受けるボーンリストにボーンとウェイトのペアを追加
			weightLists[vertIndex].emplace_back(WeightSet{ static_cast<UINT>(i), weight });
		}
	}

	// 頂点配列書き換え用の参照
	auto& vertices = model_->vertices;

	// 各頂点の処理
	for (int i = 0; i < vertices.size(); i++)
	{
		// 頂点のウェイトから最も大きい4つを選択
		auto& weightList = weightLists[i];

		// 大小比較用のラムダ式を指定して降順にソート
		weightList.sort([](auto const& lhs, auto const& rhs) 
			{
				return lhs.weight > rhs.weight; // 左の要素の方が大きければfalse
			}
		);

		int weightArrayIndex = 0;

		// 降順ソート済みのウェイトリスト
		for (auto& weightSet : weightList)
		{
			// 頂点データに書き込み
			vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
			vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;

			// 4つに達したら終了
			if (++weightArrayIndex >= FbxModel::MAX_BONE_INDICES)
			{
				float weight = 0.0f;
				// 2番目以降のウェイトを合計
				for (int j = 1; j < FbxModel::MAX_BONE_INDICES; j++)
				{
					weight += vertices[i].boneWeight[j];
				}
				// 合計で1.0f(100%)になるように調整
				vertices[i].boneWeight[0] = 1.0f - weight;
				break;
			}
		}
	}
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			dst->r[i].m128_f32[j] = static_cast<float>(src.Get(i, j));
		}
	}
}
