#include "DirectionLight.h"

class LightGroup
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static const int DirLightNum = 3;

public:
	struct ConstBufferData
	{
		// 環境光
		XMFLOAT3 ambientColor;
		float pad1;
		// 平行光源
		DirectionLight::ConstBufferData dirLights[DirLightNum];
	};

private:
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 環境光
	XMFLOAT3 ambientColor = { 1,1,1 };
	// 平行光源の配列
	DirectionLight dirLights[DirLightNum];
	// ダーティフラグ
	bool dirty = false;

public:
	void Init();
	void Update();
	void Draw(UINT rootParameterIndex);
	static LightGroup* Create();
	void TransferConstBuffer();
	void SetAmbientColor(const XMFLOAT3& color);
	void SetDirLightActive(int index, bool isActive);
	void SetDirLightDir(int index, const XMVECTOR& lightdir);
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);
	void DefaultLightSetting();
};