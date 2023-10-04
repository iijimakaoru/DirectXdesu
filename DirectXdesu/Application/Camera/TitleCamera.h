#pragma once
#include "Camera.h"

class TitleCamera : public Camera
{
public:
	TitleCamera()
	{
		
	}

	// ������
	void Init();

	// �X�V
	void Update();

	// �^�C�g���J�������[�N
	void RoundCamera();

	// ��]�X�^�[�g
	void StartRound();

	// �o���J�������[�N
	void SortieCamera();

	// �o���X�^�[�g
	void StartSortie();

private:
	// �p�x
	float rotAngle = 0;

	// ��]�J�����t���O
	bool isRound = false;

	// �o���J�����t���O
	bool isSortie = false;

	// �o���J�������o
	uint32_t sortiePhase = 0;
	float sortiePhaseTimer = 0;
	float sortiePhaseTime = 0;

	// ���̊p�x
	float nowAngle = 0;
};

