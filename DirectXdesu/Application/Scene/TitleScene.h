#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "FbxObject3D.h"

#include "BaseScene.h"

#include "KInput.h"

class TitleScene : public BaseScene
{
public:
	TitleScene()
	{
		LoadResources();
		Init();
	};
	~TitleScene();
	void LoadResources()override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Final()override;

private:
	// �C���v�b�g
	KInput* input = nullptr;

	// �J����
	std::unique_ptr<Camera> camera = nullptr;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> fbxPipeline;

	// �V�F�[�_�[
	KShader fbxShader;

	// Fbx�֘A
	FbxModel* fbxModel1 = nullptr;
	FbxObject3D* object1 = nullptr;
};

