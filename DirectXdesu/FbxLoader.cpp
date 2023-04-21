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
}
