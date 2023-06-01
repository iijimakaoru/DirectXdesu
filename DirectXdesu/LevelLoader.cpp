#include "LevelLoader.h"
#include<fstream>

const std::string LevelLoader::kDefaultBaseDirectory = "Resources/";
const std::string LevelLoader::kExtension = ".json";

LevelData* LevelLoader::LoadFile(const std::string& fileName)
{
    // �A�����ăt���p�X�𓾂�
    const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

    // �t�@�C���X�g���[��
    std::ifstream file;

    // �t�@�C�����J��
    file.open(fullpath);

    // �t�@�C���I�[�v�����s���`�F�b�N
    if (file.fail())
    {
        assert(0);
    }

    // JSON�����񂩂�𓀂����f�[�^
    nlohmann::json deserialized;

    // ��
    file >> deserialized;

    // ���������x���f�[�^�t�@�C�����`�F�b�N
    assert(deserialized.is_object());
    assert(deserialized.contains("name"));
    assert(deserialized["name"].is_string());

    // "name"�𕶎���Ƃ��Ď擾
    std::string name = deserialized["name"].get<std::string>();

    // ���������x���f�[�^�t�@�C�����`�F�b�N
    assert(name.compare("scene") == 0);

    // ���x���f�[�^�i�[�p�C���X�^���X�𐶐�
    LevelData* levelData = new LevelData();

    // "object"�̑S�I�u�W�F�N�g�𑖍�
    for (nlohmann::json& object : deserialized["objects"])
    {
        assert(object.contains("type"));

        // �������擾
        std::string type = object["type"].get<std::string>();

        // Mesh
        if (type.compare("MESH") == 0)
        {
            // �v�f�ǉ�
            levelData->objects.emplace_back(LevelData::ObjectData{});
            // �ǉ������v�f�̎Q�Ƃ𓾂�
            LevelData::ObjectData& objectData = levelData->objects.back();

            if (object.contains("file_name"))
            {
                objectData.fileName = object["file_name"];
            }

            // �g�����X�t�H�[���̃p�����[�^�ǂݍ���
            nlohmann::json& transform = object["transform"];

            // ���s�ړ�
            objectData.translation.m128_f32[0] = static_cast<float>(transform["translation"][1]);
            objectData.translation.m128_f32[1] = static_cast<float>(transform["translation"][2]);
            objectData.translation.m128_f32[2] = -static_cast<float>(transform["translation"][0]);
            objectData.translation.m128_f32[3] = 1;
            // ��]�p
            objectData.rotation.m128_f32[0] = -static_cast<float>(transform["rotation"][1]);
            objectData.rotation.m128_f32[1] = -static_cast<float>(transform["rotation"][2]);
            objectData.rotation.m128_f32[2] = static_cast<float>(transform["rotation"][0]);
            objectData.rotation.m128_f32[3] = 0;
            // �X�P�[�����O
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
