#pragma once
#include <DirectXMath.h>
#include <memory>
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>

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

	void Initialize();
	void TransferConstBuffer();
	void SetLightDir(const XMVECTOR& lightDir);
	void SetLightColor(const XMFLOAT3& lightColor);
	void Update();
	void Draw(UINT rootParameterIndex);
	static Light* Create();

private:
	ComPtr<ID3D12Resource> constBuff;

	XMVECTOR lightdir = { 1,0,0,0 };

	XMFLOAT3 lightcolor = { 1,1,1 };

	bool dirty = false;
};

