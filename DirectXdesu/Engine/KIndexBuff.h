#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>

class KIndexBuff
{
private:

	//インデックスバッファ生成が成功したか
	bool isValid = false;
	char PADING[7];

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer;

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW bufferView{};

	void* bufferMappedPtr = nullptr;

	size_t bufferLength = 0;

public:

	/// <summary>
	/// インデックスバッファを生成
	/// </summary>
	/// <param name="length">インデックスバッファの要素数</param>
	/// <param name="data">インデックス配列の先頭アドレス(uint32_t)</param>
	void Create(size_t length, const uint32_t* data = nullptr);

	/// <summary>
	/// 成功したか
	/// </summary>
	bool IsValid();

	/// <summary>
	/// インデックスバッファビューを取得
	/// </summary>
	D3D12_INDEX_BUFFER_VIEW GetView() const;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	void Update(void* data);

	KIndexBuff() = default;
	~KIndexBuff() = default;

private:

	KIndexBuff(const KIndexBuff&) = delete;
	void operator = (const KIndexBuff&) = delete;
};

