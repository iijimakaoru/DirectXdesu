#include "Player.h"
#include "Ease.h"
#include "BulletManager.h"
#include "ParticleManager.h"

const float Player::moveSpeed = 0.48f;
const KMyMath::Vector2 Player::rotLimit = { 35.0f, 25.0f };
const KMyMath::Vector2 Player::posLimitMin = { -15.0f, -4.0f };
const KMyMath::Vector2 Player::posLimitMax = { 15.0f, Player::posLimitMin.y + 12.0f };

Player* Player::Create(KModel* model_, KGPlin* objPipeline_, const float playerHP, KGPlin* spritePipeline_)
{
	// �C���X�^���X����
	Player* player = new Player();
	if (player == nullptr)
	{
		return nullptr;
	}

	// ������
	player->Init(model_, objPipeline_, playerHP, spritePipeline_);

	return player;
}

void Player::Init(KModel* model_, KGPlin* objPipeline_, const float playerHP, KGPlin* spritePipeline_)
{
	input = KInput::GetInstance();

	// ���f������
	model = model_;

	// �p�C�v���C������
	objPipeline = objPipeline_;
	spritePipeline = spritePipeline_;

	// �I�u�W�F�N�g����
	object3d.reset(KObject3d::Create(model, objPipeline));
	object3d->transform.pos.z = 50;
	object3d->transform.scale = { 2.0f,2.0f,2.0f };

	// ���e�B�N��
	reticle3d = std::make_unique<Reticle3D>();
	reticle3d->Init();
	reticle2d = std::make_unique<Reticle2D>();
	reticle2d->Init();

	// HP�֘A
	maxHP = playerHP;
	HP = maxHP;

	HPUI = std::make_unique<Sprite>();
	HPUI->Init();
	HPUI->SetPipeline(spritePipeline);

	hpTex = TextureManager::Load("Resources/texture/white1x1.png");

	HPBarUI = std::make_unique<Sprite>();
	HPBarUI->Init();
	HPBarUI->SetPipeline(spritePipeline);

	hpbarTex = TextureManager::Load("Resources/texture/PlayersHPBar.png");

	isDead = false;
}

void Player::Update(ViewProjection* viewPro)
{
	// ���S����
	if (HP <= min(HP, 0))
	{
		isDead = true;
	}

	if (!isDead)
	{
		// �ړ�
		Move();

		// ��]
		Rot();

		// �U��
		Attack();
	}
	else
	{
		DeadEffect();
	}

	// 3D���e�B�N���̍X�V
	reticle3d->Update(object3d->transform.matWorld, GetWorldPos());

	// 2D���e�B�N���̍X�V
	reticle2d->Update(viewPro, reticle3d->GetWorldPos());

	// �I�u�W�F�N�g�̍X�V
	object3d->Update(viewPro);
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
	object3d->transform.rot.y = min(object3d->transform.rot.y, rotLimit.y);
	object3d->transform.rot.x = max(object3d->transform.rot.x, -rotLimit.x);
	object3d->transform.rot.x = min(object3d->transform.rot.x, rotLimit.x);
}

void Player::Attack()
{
	if (input->GetPadButtonDown(XINPUT_GAMEPAD_A))
	{
		const float bulletSpeed = 6.0f;
		KMyMath::Vector3 bulletVec(0, 0, 1);

		const float distance = 20.0f;

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]
		bulletVec = MyMathUtility::TransforNormal(bulletVec, object3d->transform.matWorld);

		bulletVec = MyMathUtility::MakeNormalize(bulletVec);

		// �e����
		BulletManager::GetInstance()->PlayerBulletShot(GetWorldPos() + bulletVec * distance, bulletVec, object3d->transform.rot, bulletSpeed);
	}
}

void Player::DeadEffect()
{
	if (!isFallEffectEnd)
	{
		// �p������
		object3d->transform.rot.x = 25.0f;
		object3d->transform.rot.y = 0;

		// ��]
		object3d->transform.rot.z += 10.0f;

		// ����
		object3d->transform.pos.y -= 0.1f;

		// ���Ԍo��
		fallEffectTimer++;

		// 
		expTimer++;

		if (expTimer >= max(expTimer, expTime))
		{
			ParticleManager::GetInstance()->CallSmallExp({ GetWorldPos().x + MyMathUtility::GetRand(-1.0f,1.0f),
			GetWorldPos().y + MyMathUtility::GetRand(-1.0f,1.0f),GetWorldPos().z + MyMathUtility::GetRand(-1.0f,1.0f) });
			expTimer = 0;
		}

		// ���o�I���
		if (fallEffectTimer >= max(fallEffectTimer, fallEffectTime))
		{
			ParticleManager::GetInstance()->CallExp(GetWorldPos());
			isFallEffectEnd = true;
		}
	}
}

void Player::ObjDraw()
{
	if (!isFallEffectEnd)
	{
		object3d->Draw();
	}
}

void Player::SpriteDraw()
{
	if (!isDead)
	{
		reticle2d->Draw();
	}
}

void Player::UIDraw()
{
	HPBarUI->Draw(hpbarTex, { 10,10 }, { 1,1 }, 0, { 1,1,1,1 }, false, false, { 0,0 });

	HPUI->Draw(hpTex, { 11,11 }, { HP * (318 / maxHP),30 }, 0, { 0,1,0,1 }, false, false, { 0,0 });
}

void Player::SetParent(const WorldTransfom* parent)
{
	object3d->transform.parent = parent;
}

const KMyMath::Vector3& Player::GetPosition() const
{
	return object3d->transform.pos;
}

const KMyMath::Vector3 Player::GetWorldPos() const
{
	// ���[���h���W�i�[�ϐ�
	KMyMath::Vector3 result;

	// ���[���h�s��̕��s�ړ������擾
	result.x = object3d->transform.matWorld.m[3][0];
	result.y = object3d->transform.matWorld.m[3][1];
	result.z = object3d->transform.matWorld.m[3][2];

	return result;
}

const KMyMath::Vector3 Player::GetRot() const
{
	return object3d->transform.rot;
}

const bool Player::GetIsDead() const
{
	return isDead;
}

const bool Player::GetIsFallEffectEnd() const
{
	return isFallEffectEnd;
}

void Player::OnCollision()
{
	ParticleManager::GetInstance()->CallExp(GetWorldPos());
	HP--;
}
