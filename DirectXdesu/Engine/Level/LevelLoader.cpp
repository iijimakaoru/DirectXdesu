#include "LevelLoader.h"
#include<fstream>

using namespace MesiEngine;

const std::string LevelLoader::kDefaultBaseDirectory = "Resources/";
const std::string LevelLoader::kExtension = ".json";

LevelData* LevelLoader::LoadFile(const std::string& fileName)
{
    // 連結してフルパスを得る
    const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

    // ファイルストリーム
    std::ifstream file;

    // ファイルを開く
    file.open(fullpath);

    // ファイルオープン失敗をチェック
    if (file.fail())
    {
        assert(0);
    }

    // JSON文字列から解凍したデータ
    nlohmann::json deserialized;

    // 解凍
    file >> deserialized;

    // 正しいレベルデータファイルかチェック
    assert(deserialized.is_object());
    assert(deserialized.contains("name"));
    assert(deserialized["name"].is_string());

    // "name"を文字列として取得
    std::string name = deserialized["name"].get<std::string>();

    // 正しいレベルデータファイルかチェック
    assert(name.compare("scene") == 0);

    // レベルデータ格納用インスタンスを生成
    LevelData* levelData = new LevelData();

    // "object"の全オブジェクトを走査
    for (nlohmann::json& object : deserialized["objects"])
    {
        assert(object.contains("type"));

        // 権利を取得
        std::string type = object["type"].get<std::string>();

        // Mesh
        if (type.compare("MESH") == 0)
        {
            // 要素追加
            levelData->objects.emplace_back(LevelData::ObjectData{});
            // 追加した要素の参照を得る
            LevelData::ObjectData& objectData = levelData->objects.back();

            if (object.contains("file_name"))
            {
                objectData.fileName = object["file_name"];
            }

            // トランスフォームのパラメータ読み込み
            nlohmann::json& transform = object["transform"];

            // 平行移動
            objectData.translation.m128_f32[0] = static_cast<float>(transform["translation"][1]);
            objectData.translation.m128_f32[1] = static_cast<float>(transform["translation"][2]);
            objectData.translation.m128_f32[2] = -static_cast<float>(transform["translation"][0]);
            objectData.translation.m128_f32[3] = 1;
            // 回転角
            objectData.rotation.m128_f32[0] = -static_cast<float>(transform["rotation"][1]);
            objectData.rotation.m128_f32[1] = -static_cast<float>(transform["rotation"][2]);
            objectData.rotation.m128_f32[2] = static_cast<float>(transform["rotation"][0]);
            objectData.rotation.m128_f32[3] = 0;
            // スケーリング
            objectData.scaling.m128_f32[0] = static_cast<float>(transform["scaling"][1]);
            objectData.scaling.m128_f32[1] = static_cast<float>(transform["scaling"][2]);
            objectData.scaling.m128_f32[2] = static_cast<float>(transform["scaling"][0]);
            objectData.scaling.m128_f32[3] = 0;
        }

        if (object.contains("children"))
        {

        }
    }

    return levelData;
}
