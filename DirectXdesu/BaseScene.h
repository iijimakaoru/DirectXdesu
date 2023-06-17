#pragma once
#include "Camera.h"

#include<memory>

class BaseScene
{
public: // �����o�֐�
	virtual ~BaseScene() = default;

	// ������
	virtual void Init() = 0;

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// �I��
	virtual void Final() = 0;

	// ���\�[�X�ǂݍ���
	virtual void LoadResources() = 0;

protected:
	std::unique_ptr<Camera> camera = nullptr;
};

