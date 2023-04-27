#include "FbxLoader.h"
#include <cassert>

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const std::string FbxLoader::baseDirectory = "Resources/";

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

void FbxLoader::LoadModelFromFile(const string& modelName)
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
    fbxScene->Destroy();
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

    // 子ノードに対して再帰呼び出し
    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}
