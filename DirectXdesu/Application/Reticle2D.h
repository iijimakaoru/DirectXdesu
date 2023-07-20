#pragma once
#include "Sprite.h"
#include "ViewProjection.h"

class Reticle2D
{
public:
	void Init();

	void Update(ViewProjection* viewPro,KMyMath::Vector3 reticle3dPos);

	void Draw();

	// �|�W�V�����Q�b�^�[
	const KMyMath::Vector2 GetPos() const;

protected:
	// �X�v���C�g
	std::unique_ptr<Sprite> reticle = nullptr;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> pipeline;

	// �V�F�[�_�[
	KShader shader;

	// �e�N�X�`��
	TextureData texData;

	KMyMath::Vector2 position;

	KMyMath::Vector2 scale;

	KMyMath::Vector4 color;

	float rot;
};