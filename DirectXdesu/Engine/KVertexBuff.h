#pragma once
#include <d3d12.h>
#include <wrl.h>

class KVertexBuff
{
private:
	// バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};

	void* bufferMappedPtr = nullptr;

	// バッファの生成に成功したか
	bool isValid = false;
	char PADING[7];
public:

	/// <summary>
	/// バッファを生成
	/// </summary>
	/// <param name="length">要素数</param>
	/// <param name="singleSize">単体のサイズ</param>
	/// <param name="data">配列の先頭アドレス</param>
	void Create(size_t length, size_t singleSize, const void* data = nullptr);

	/// <summary>
	/// 頂点バッファビューを取得
	/// </summary>
	/// <returns>頂点バッファビュー</returns>
	D3D12_VERTEX_BUFFER_VIEW GetView() const;

	/// <summary>
	/// バッファを取得
	/// </summary>
	/// <returns>バッファ</returns>
	ID3D12Resource* GetResource();

	/// <summary>
	/// バッファの生成に成功したかを取得
	/// </summary>
	/// <returns>成功したか</returns>
	bool IsValid();

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	void Update(void* data);

	~KVertexBuff() = default;
	KVertexBuff() = default;

private:

	KVertexBuff(const KVertexBuff&) = delete;

	void operator = (const KVertexBuff&) = delete;
};

