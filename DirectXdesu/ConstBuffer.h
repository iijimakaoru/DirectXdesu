#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Vector4.h"
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")

using namespace DirectX;

struct ConstBufferData
{
	XMFLOAT4 color; // êF
	XMMATRIX mat; // 3Dïœä∑çsóÒ
};

struct ConstBufferDataB0
{
	XMMATRIX mat;
};

struct ConstBufferDataB1
{
	XMFLOAT3 ambient;
	float pad1;
	XMFLOAT3 diffuse;
	float pad2;
	XMFLOAT3 specular;
	float alpha;
};

class ConstBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	HRESULT result;
	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};

	struct ConstBufferDataMaterial
	{
		KMyMath::Vector4 color;
	};

	struct ConstBufferTimeMaterial
	{
		float time;
	};

	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* mapMaterial = nullptr;
	ComPtr<ID3D12Resource> timeMaterial;
	ConstBufferTimeMaterial* timeMapMaterial = nullptr;
	float timer = 100000;
public:
	ConstBuff();
	~ConstBuff();
	void ChangeColor(const KMyMath::Vector4& color);
	void TimeUpdate();
	void Update();
	void SetBufferView();
};