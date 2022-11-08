#include "Sprite.h"
#include "KShader.h"

void Sprite::Init(SpriteCommon* spriteCommon)
{
	assert(spriteCommon);
	spriteCommon_ = spriteCommon;


}

void Sprite::SpriteTransferVertexBuffer(const SpriteInfo& sprite, const SpriteCommond& spriteCommon)
{

}

SpriteInfo Sprite::SpriteCreate(UINT texNumber, const SpriteCommond& spriteCommon, Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	return SpriteInfo();
}

void Sprite::SpriteDraw(const SpriteInfo& sprite, ID3D12GraphicsCommandList* cmdList, const SpriteCommond& spriteCommon)
{
}

void Sprite::SpriteUpdate(SpriteInfo& sprite, const SpriteCommond& spriteCommon)
{
}
