#pragma once
#include <wrl.h>
#include <d3d12.h>

class CommandSignature
{
private:
	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSignature = nullptr;

public:
	void Create();

	ID3D12CommandSignature* GetCommandSignature();
};

