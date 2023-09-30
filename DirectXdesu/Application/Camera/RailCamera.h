#pragma once
#include "Camera.h"
#include "KObject3d.h"

class Player;

class RailCamera : public Camera
{
public:
	RailCamera() 
	{
		Init();
	};

	void Init();

	void Update(Player* player);

	/// <summary>
	/// �J����������
	/// </summary>
	void Move();

	/// <summary>
	/// �v���C���[�̊p�x�ƘA��������
	/// </summary>
	/// <param name="playersRot"></param>
	void SetRot(const KMyMath::Vector3& playersRot);

	/// <summary>
	/// �O�i�t���O
	/// </summary>
	/// <param name="isAdvance_"></param>
	void SetIsAdvance(bool isAdvance_)
	{
		isAdvance = isAdvance_;
	}

	/// <summary>
	/// �J�������x�Z�b�^�[
	/// </summary>
	/// <returns></returns>
	const float GetSpeed() const;

	/// <summary>
	/// ���񂾂Ƃ��ɌĂڂ�
	/// </summary>
	void CallCrash();

private:
	// �O�i���邩
	bool isAdvance = true;

	// �O�i�X�s�[�h
	static const float advanceSpeed;

	// �㉺���E���̃J�����̑���(���@�̉��{)
	static const float moveSpeedPlayerMagnification;

	// �ړ����
	KMyMath::Vector2 moveLimitMin = {};
	KMyMath::Vector2 moveLimitMax = {};

	// �v���C���[����ꂽ��Ă�
	bool isCrash = false;

	// �Q�[���N���A�ŌĂ�
	bool isStageClear = false;
};