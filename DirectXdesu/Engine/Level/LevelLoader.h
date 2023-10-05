#pragma once
#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 4626)
#pragma warning(disable: 5027)
#pragma warning(disable: 4623)
#pragma warning(disable: 4668)
#pragma warning(disable: 4625)
#pragma warning(disable: 4514)
#include "json.hpp"
#include <DirectXMath.h>
#pragma warning(pop)

// ���x���f�[�^
struct LevelData 
{
	struct ObjectData 
	{
		// �t�@�C����
		std::string fileName;
		// ���s�ړ�
		DirectX::XMVECTOR translation;
		// ��]�p
		DirectX::XMVECTOR rotation;
		// �X�P�[�����O
		DirectX::XMVECTOR scaling;
	};

	// �I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;
};

// ���x���f�[�^�̃��[�_�[
class LevelLoader 
{
public:// �萔
	// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string kDefaultBaseDirectory;
	// �t�@�C���g���q
	static const std::string kExtension;

public:// �����o�֐�

	/// <summary>
	/// ���x���f�[�^�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	static LevelData* LoadFile(const std::string& fileName);
};

