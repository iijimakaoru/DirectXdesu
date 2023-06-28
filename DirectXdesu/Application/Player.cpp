#include "Player.h"
#include "Ease.h"

void Player::Init()
{
	input = KInput::GetInstance();

	// ���f������
	model = std::make_unique<Cube>();
	model->CreateModel();

	// �e�N�X�`������
	tex.CreateTexture("Resources/texture/", "mario.jpg");

	// �p�C�v���C������
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline = std::make_unique<KGPlin>();
	pipeline->CreatePipelineAll(shader, true);

	// �I�u�W�F�N�g����
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model.get());
	object3d->transform.scale = { 2.0f,2.0f,2.0f };
}

void Player::Update(ViewProjection& viewPro)
{
	leftStickPos = input->GetPadLStick();

	Move();
	Rot();

	object3d->Update(viewPro);
}

void Player::Move()
{
	//���@���X���Ă���p�x�Ɉړ�������
	KMyMath::Vector3 velocity = { 0, 0, 0 };
	velocity.x = moveSpeed * (object3d->transform.rot.y / rotLimit.y);
	velocity.y = moveSpeed * -(object3d->transform.rot.x / rotLimit.x);
	object3d->transform.pos += velocity;

	const float moveLimitX = 60;
	const float moveLimitY = 30;

	object3d->transform.pos.x = max(object3d->transform.pos.x, -moveLimitX);
	object3d->transform.pos.x = min(object3d->transform.pos.x, moveLimitX);
	object3d->transform.pos.y = max(object3d->transform.pos.y, -moveLimitY);
	object3d->transform.pos.y = min(object3d->transform.pos.y, moveLimitY);
}

void Player::Rot()
{
	//��]���x
	const float rotSpeed = 0.5f;

	//�p�x�C������x
	const float correctionSpeed = rotSpeed / 6.0f;
	KMyMath::Vector3 rot = { 0, 0, 0 };

	//�ǂ��܂ŌX�����画����Ƃ邩
	const float stickNum = 200;

	// Y����]
	if (input->LStickTiltX(stickNum) || input->LStickTiltX(-stickNum))
	{
		//���@�̓X�e�B�b�N��|���������ɓ���
		const float stickRota = input->GetLStickAngle();
		const float moveAngle = XMConvertToRadians(stickRota);
		const float padStickIncline = input->GetLStickInline().x;
		rot.y = rotSpeed * cosf(moveAngle) * fabsf(padStickIncline);
	}
	else
	{
		//�p�x�C�����x�{��
		float backSpeedRatio = fabsf(object3d->transform.rot.y / (rotLimit.y * 2)) + 0.5f;
		//�p�x�C�����x
		const float backSpeed = correctionSpeed * backSpeedRatio;
		//y����]�̌X�����C������
		const float rotMin = 0.5f;
		if (object3d->transform.rot.y > rotMin)
		{
			rot.y -= backSpeed;
		}
		else if (object3d->transform.rot.y < -rotMin)
		{
			rot.y += backSpeed;
		}
		else
		{
			object3d->transform.rot.y = 0;
		}
	}

	// X����]
	if (input->LStickTiltY(stickNum) || input->LStickTiltY(-stickNum))
	{
		//���@�̓X�e�B�b�N��|���������ɓ���
		const float stickRota = -input->GetLStickAngle();
		const float moveAngle = XMConvertToRadians(stickRota);
		const float padStickIncline = input->GetLStickInline().y;
		rot.x = rotSpeed * sinf(moveAngle) * fabsf(padStickIncline);
	}
	else
	{
		//�p�x�C�����x�{��
		float backSpeedRatio = fabsf(object3d->transform.rot.x / (rotLimit.x * 2)) + 0.5f;
		//�p�x�C�����x
		const float backSpeed = correctionSpeed * backSpeedRatio;
		//y����]�̌X�����C������
		const float rotMin = 0.5f;
		if (object3d->transform.rot.x > rotMin)
		{
			rot.y -= backSpeed;
		}
		else if (object3d->transform.rot.x < -rotMin)
		{
			rot.y += backSpeed;
		}
		else
		{
			object3d->transform.rot.x = 0;
		}
	}

	object3d->transform.rot = rot;

	// �p�x����
	object3d->transform.rot.y = max(object3d->transform.rot.y, -rotLimit.y);
	object3d->transform.rot.y = min(object3d->transform.rot.y,  rotLimit.y);
	object3d->transform.rot.x = max(object3d->transform.rot.x, -rotLimit.x);
	object3d->transform.rot.x = min(object3d->transform.rot.x,  rotLimit.x);
}

void Player::Draw()
{
	object3d->Draw(&tex);
}