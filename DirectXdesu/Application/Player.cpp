#include "Player.h"
#include "Ease.h"

const float Player::moveSpeed = 0.32f;
const KMyMath::Vector2 Player::rotLimit = { 35.0f, 25.0f };
const KMyMath::Vector2 Player::posLimitMin = { -15.0f, -4.0f };
const KMyMath::Vector2 Player::posLimitMax = { 15.0f, Player::posLimitMin.y + 12.0f };

void Player::Init(KModel* model_)
{
	input = KInput::GetInstance();

	// ���f������
	model = model_;

	// �p�C�v���C������
	pipeline = std::make_unique<KGPlin>();
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline->CreatePipelineAll(shader, "Obj");

	// �I�u�W�F�N�g����
	object3d = std::make_unique<KObject3d>();
	object3d->Initialize();
	object3d->SetPipeline(pipeline.get());
	object3d->LoadModel(model);
	object3d->transform.pos.z = 50;
	object3d->transform.scale = { 2.0f,2.0f,2.0f };

	// ���e�B�N��
	reticle3d = std::make_unique<Reticle3D>();
	reticle3d->Init();
	reticle2d = std::make_unique<Reticle2D>();
	reticle2d->Init();
}

void Player::Update(ViewProjection* viewPro)
{
	// �e�폜
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->GetIsDead();
		});

	Move();
	Rot();

	Attack();

	reticle3d->Update(object3d->transform.matWorld, GetWorldPos());

	reticle2d->Update(viewPro,reticle3d->GetWorldPos());

	object3d->Update(viewPro);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Update(viewPro);
	}
}

void Player::Move()
{
	//���@���X���Ă���p�x�Ɉړ�������
	KMyMath::Vector3 velocity = { 0, 0, 0 };
	velocity.x = (object3d->transform.rot.y / rotLimit.y);
	velocity.y = -(object3d->transform.rot.x / rotLimit.x);

	object3d->transform.pos.x += velocity.x * moveSpeed;
	object3d->transform.pos.y += velocity.y * moveSpeed;

	object3d->transform.pos.x = max(object3d->transform.pos.x, posLimitMin.x);
	object3d->transform.pos.x = min(object3d->transform.pos.x, posLimitMax.x);
	object3d->transform.pos.y = max(object3d->transform.pos.y, posLimitMin.y);
	object3d->transform.pos.y = min(object3d->transform.pos.y, posLimitMax.y);
}

void Player::Rot()
{
	//��]���x
	const float rotSpeed = 0.025f;

	//�p�x�C������x
	const float correctionSpeed = (rotSpeed * 2.0f) / 0.1f;
	KMyMath::Vector3 rot = { 0, 0, 0 };

	//�ǂ��܂ŌX�����画����Ƃ邩
	const float stickNum = 100;

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
			rot.x -= backSpeed;
		}
		else if (object3d->transform.rot.x < -rotMin)
		{
			rot.x += backSpeed;
		}
		else
		{
			object3d->transform.rot.x = 0;
		}
	}

	// Z����]
	{
		const float rotZspeed = 0.01f;
		const float rotZLimit = 1.0f;
		//�E��]
		if (isRotZRight) 
		{
			swayZ += rotZspeed;
			if (swayZ >= rotZLimit) 
			{
				isRotZRight = false;
			}
		}
		//����]
		else 
		{
			swayZ -= rotZspeed;
			if (swayZ <= -rotZLimit) 
			{
				isRotZRight = true;
			}
		}

		object3d->transform.rot.z = -object3d->transform.rot.y + swayZ;
	}

	object3d->transform.rot += rot;

	// �p�x����
	object3d->transform.rot.y = max(object3d->transform.rot.y, -rotLimit.y);
	object3d->transform.rot.y = min(object3d->transform.rot.y,  rotLimit.y);
	object3d->transform.rot.x = max(object3d->transform.rot.x, -rotLimit.x);
	object3d->transform.rot.x = min(object3d->transform.rot.x,  rotLimit.x);
}

void Player::Attack()
{
	if (input->GetPadButtonDown(XINPUT_GAMEPAD_A))
	{
		const float bulletSpeed = 6.0f;
		KMyMath::Vector3 bulletVec(0, 0, 1);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]
		bulletVec = MyMathUtility::TransforNormal(bulletVec, object3d->transform.matWorld);

		// �e����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		// ������
		newBullet->Init();
		// �z�u
		newBullet->Set(GetWorldPos(), bulletVec, object3d->transform.rot, bulletSpeed);
		// �o�^
		bullets.push_back(std::move(newBullet));
	}
}

void Player::Draw()
{
	object3d->Draw();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Draw();
	}

	reticle2d->Draw();
}

void Player::SetParent(const WorldTransfom* parent)
{
	object3d->transform.parent = parent;
}

KMyMath::Vector3 Player::GetWorldPos()
{
	// ���[���h���W�i�[�ϐ�
	KMyMath::Vector3 result;

	// ���[���h�s��̕��s�ړ������擾
	result.x = object3d->transform.matWorld.m[3][0];
	result.y = object3d->transform.matWorld.m[3][1];
	result.z = object3d->transform.matWorld.m[3][2];

	return result;
}

void Player::OnCollision()
{
}
