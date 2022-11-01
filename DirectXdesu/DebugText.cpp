#include "DebugText.h"

void DebugText::Init(ID3D12Device* dev, int window_width, int window_height,
	UINT texnumber, const SpriteCommond& spriteCommon)
{
	sprite = std::make_unique<Sprite>();

	for (int i = 0; i < _countof(sprites); i++)
	{
		sprites[i] = sprite->SpriteCreate(dev, window_width, window_height,
			texnumber, spriteCommon, { 0,0 });
	}
}

void DebugText::Print(const SpriteCommond& spriteCommon, const std::string& text,
	Vector2 pos, float scale)
{
	for (int i = 0; i < text.size(); i++)
	{
		if (spriteIndex >= maxCharCount)
		{
			break;
		}

		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		sprites[spriteIndex].position = { pos.x + fontWidth * scale * i,pos.y,0 };
		sprites[spriteIndex].texLeftTop = { (float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight };
		sprites[spriteIndex].texSize = { fontWidth,fontHeight };
		sprites[spriteIndex].size = { fontWidth * scale,fontHeight * scale };

		SpriteTransferVertexBuffer(sprites[spriteIndex], spriteCommon);

		sprite->SpriteUpdate(sprites[spriteIndex], spriteCommon);

		spriteIndex++;
	}
}

void DebugText::SpriteTransferVertexBuffer(const SpriteInfo& sprite, const SpriteCommond& spriteCommon)
{
	HRESULT result = S_FALSE;

	VertexPosUV vertices[] = {
		{{},{0.0f,1.0f}}, // 左下
		{{},{0.0f,0.0f}}, // 左上
		{{},{1.0f,1.0f}}, // 右下
		{{},{1.0f,0.0f}}, // 右上
	};

	enum { LB, LT, RB, RT };

	float left = (0.0f - sprite.anchorpoint.x) * sprite.size.x;
	float right = (1.0f - sprite.anchorpoint.x) * sprite.size.x;
	float top = (0.0f - sprite.anchorpoint.y) * sprite.size.y;
	float bottom = (1.0f - sprite.anchorpoint.y) * sprite.size.y;

	if (sprite.isFlipX)
	{
		left = -left;
		right = -right;
	}

	if (sprite.isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,	  top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,	  top,0.0f };

	if (spriteCommon.texBuff[sprite.texNum])
	{
		// 
		D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[sprite.texNum]->GetDesc();

		float tex_left = sprite.texLeftTop.x / resDesc.Width;
		float tex_right = (sprite.texLeftTop.x + sprite.texSize.x) / resDesc.Width;
		float tex_top = sprite.texLeftTop.y / resDesc.Height;
		float tex_bottom = (sprite.texLeftTop.y + sprite.texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };
	}

	// 頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
}

void DebugText::DrawAll(ID3D12Device* dev, const SpriteCommond& spriteCommon,
	ID3D12GraphicsCommandList* cmdList)
{
	for (int i = 0; i < spriteIndex; i++)
	{
		sprite->SpriteDraw(sprites[i], cmdList, spriteCommon, dev);
	}

	spriteIndex = 0;
}
