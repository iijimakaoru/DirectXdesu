#pragma once
#include <d3d12.h>

enum BlendMode {
	NONE,
	ADD,
	SUB,
	INV,
	ALPHA
};

D3D12_BLEND_DESC CreateBlend(BlendMode mode);

