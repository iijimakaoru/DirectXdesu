#pragma once
#include "Framework.h"

class KMyGame : public Framework
{
public:
	// ������
	void Init() override;
	// �X�V
	void Update() override;
	// �`��
	void Draw() override;
	// �I��
	void Final() override;

private:
};

