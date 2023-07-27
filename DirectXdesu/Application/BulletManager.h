#pragma once
#include "PlayerBullet.h"
#include "EnemyBullet.h"

class BulletManager
{
public:
	/// <summary>
	/// �e�̍X�V
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro);

	/// <summary>
	/// �e�̕`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �v���C���[�̒e����
	/// </summary>
	/// <param name="model"></param>
	/// <param name="pipeline_"></param>
	/// <param name="pos"></param>
	/// <param name="vec_"></param>
	/// <param name="rot_"></param>
	/// <param name="bulletSpeed"></param>
	void PlayerBulletShot(KModel* model, KGPlin* pipeline_,
		const KMyMath::Vector3& pos, const KMyMath::Vector3& vec_, const KMyMath::Vector3& rot_, const float bulletSpeed);

	void EnemyBulletShot();

	// �C���X�^���X�Q�b�^�[
	static BulletManager* GetInstance();

	// ���
	void Delete();

	// ���e�̃��X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetPlayerBullets() const;

	// �G�e�̃��X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetEnemyBullets() const;

private:
	// �e�̍폜
	void DeleteBullet();

private:
	// �v���C���[�̒e
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;

	// �G�̒e
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;

	// �C���X�^���X
	static BulletManager* bulletManager;

private:
	BulletManager() = default;
	~BulletManager() = default;
	BulletManager(const BulletManager&) = delete;
	const BulletManager& operator=(const BulletManager&) = delete;
};

