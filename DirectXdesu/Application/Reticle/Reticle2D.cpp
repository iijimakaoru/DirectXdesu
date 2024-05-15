#include "Reticle2D.h"
#include "KWinApp.h"
#include "PipelineManager.h"

void Reticle2D::Init() {
	// テクスチャ生成
	texData = TextureManager::Load("Resources/texture/reticle.png");

	reticle.reset(Sprite::Create(PipelineManager::GetInstance()->GetPipeline("Sprite")));

	scale_ = {0.5f, 0.5f};

	color = {1.0f, 1.0f, 1.0f, 1.0f};

	rot_ = 0;
}

void Reticle2D::Update(ViewProjection* viewPro, KMyMath::Vector3 reticle3dPos_) {
	KMyMath::Vector3 positionReticle = reticle3dPos_;

	float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	// ビューポート行列
	DirectX::XMMATRIX matViewport = {
	    {width / 2, 0,           0, 0},
        {0,         -height / 2, 0, 0},
        {0,         0,           1, 0},
        {width / 2, height / 2,  0, 1}
    };

	//
	KMyMath::Matrix4 matViewProjectionViewport =
	    viewPro->GetMatView() * viewPro->GetMatPro() *
	    MyMathConvert::ChangeXMMATRIXtoMatrix4(matViewport);

	positionReticle =
	    MyMathUtility::MatrixTransformWDivision(positionReticle, matViewProjectionViewport);

	position = {positionReticle.x, positionReticle.y};
}

void Reticle2D::Draw() {
	reticle->Draw(texData, position, scale_, rot_, color, false, false, {0.5f, 0.5f});
}

const KMyMath::Vector2 Reticle2D::GetPos() const { return position; }

void Reticle2D::SetRot(const float& rot) { rot_ = rot; }

void Reticle2D::SetScale(const KMyMath::Vector2& scale) { scale_ = scale; }

void Reticle2D::SetColor(const KMyMath::Vector3& rgb_) {
	color.x = rgb_.x;
	color.y = rgb_.y;
	color.z = rgb_.z;
}
