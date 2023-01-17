#pragma once
#include <DirectXMath.h>
#include <memory>
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")

using namespace DirectX;

using namespace Microsoft::WRL;

class Light
{
public:
	struct  ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
	};

	static void Staticlnitialize(ID3D12Device* device);

	void Initialize();

private:
	static ID3D12Device* device;

	ComPtr<ID3D12Resource> constBuff;

	XMVECTOR lightdir = { 1,0,0,0 };

	XMFLOAT3 lightcolor = { 1,1,1 };

	bool dirty = false;
};

