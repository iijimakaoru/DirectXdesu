#include "Sprite.h"
#include "KShader.h"

void Sprite::Init(SpriteCommon* spriteCommon)
{
	assert(spriteCommon);
	spriteCommon_ = spriteCommon;
	//vbView = spriteCommon_->GetVbView();
}

void Sprite::Draw()
{
	spriteCommon_->Draw();
	/*spriteCommon_->GetCmdList()->SetPipelineState(spriteCommon_->GetPipelineState());
	spriteCommon_->GetCmdList()->SetGraphicsRootSignature(spriteCommon_->GetRootSignature());

	spriteCommon_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	spriteCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	spriteCommon_->GetCmdList()->DrawInstanced(spriteCommon_->GetVertices().size(), 1, 0, 0);*/
}
