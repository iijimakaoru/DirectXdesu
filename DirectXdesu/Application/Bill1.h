#pragma once
#include <memory>
#include "Camera.h"
#include "KObject3d.h"
#include "KModel.h"
#include "KGPlin.h"
#include "KShader.h"

class Bill1
{
public:
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="objPipeline_"></param>
	/// <returns></returns>
	static Bill1* Create(KModel* model_, KGPlin* objPipeline_, const KMyMath::Vector2& pos);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="objPipeline_"></param>
	void Init(KModel* model_, KGPlin* objPipeline_, const KMyMath::Vector2 pos);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro, const float& cameraZ);

	// �`��
	void Draw();

	// 
	const bool GetIsDead() const;

private:
	// �I�u�W�F�N�g
	std::unique_ptr<KObject3d> object;

	// ���f��
	KModel* model = nullptr;

	// �p�C�v���C��
	KGPlin* objPipeline;

	bool isDead = false;
};

