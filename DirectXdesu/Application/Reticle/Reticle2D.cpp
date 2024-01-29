#include "Reticle2D.h"
#include "KWinApp.h"
#include "PipelineManager.h"

void Reticle2D::Init()
{
	// テクスチャ生成
	texData = TextureManager::Load("Resources/texture/reticle.png");

	reticle.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
}

void Reticle2D::Update(ViewProjection* viewPro_, KMyMath::Vector3 reticle3dPos_)
{
	KMyMath::Vector3 positionReticle = reticle3dPos_;

	float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	// ビューポート行列
	DirectX::XMMATRIX matViewport = 
	{
		{width / 2, 0, 0, 0 },
		{0, -height / 2, 0, 0},
		{0, 0, 1, 0},
		{width / 2, height / 2, 0, 1}
	};

	// 
	KMyMath::Matrix4 matViewProjectionViewport =
		viewPro_->GetMatView() * 
		viewPro_->GetMatPro() * 
		MyMathConvert::ChangeXMMATRIXtoMatrix4(matViewport);

	positionReticle = MyMathUtility::MatrixTransformWDivision(positionReticle, matViewProjectionViewport);

	position = { positionReticle.x, positionReticle.y };

	scale = { 0.5f,0.5f };

	color = { 1.0f,1.0f,1.0f,1.0f };

	rot = 0;
}

void Reticle2D::Draw()
{
	reticle->Draw(texData, position, scale, rot, color, false, false, { 0.5f,0.5f });
}

const KMyMath::Vector2 Reticle2D::GetPos() const
{
	return position;
}
