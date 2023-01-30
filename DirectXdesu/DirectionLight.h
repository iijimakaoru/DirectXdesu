#pragma once
#include <DirectXMath.h>
#include <memory>
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d12.lib")

using namespace DirectX;

using namespace Microsoft::WRL;

class DirectionLight
{
public:
	struct  ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
		unsigned int active;
	};

	void Initialize();
	void TransferConstBuffer();
	void SetLightDir(const XMVECTOR& lightDir);
	void SetLightColor(const XMFLOAT3& lightColor);
	XMVECTOR GetLightDir() { return lightdir; }
	XMFLOAT3 GetLightColor() { return lightcolor; }
	void Update();
	void Draw(UINT rootParameterIndex);
	static DirectionLight* Create();
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }

private:
	ComPtr<ID3D12Resource> constBuff;

	XMVECTOR lightdir = { 1,0,0,0 };

	XMFLOAT3 lightcolor = { 1,1,1 };

	bool dirty = false;

	bool active = false;
};

