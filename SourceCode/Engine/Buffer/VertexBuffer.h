#pragma once
//頂点バッファ
#include<wrl.h>
#include <d3d12.h>

using namespace Microsoft::WRL;

class VertexBuffer
{
public:

	VertexBuffer() = default;
	~VertexBuffer() = default;

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="length"></param>
	/// <param name="singleSize"></param>
	/// <param name="data"></param>
	void Create(size_t length, size_t singleSize, const void* data = nullptr);

	/// <summary>
	/// バッファビュー取得
	/// </summary>
	/// <returns></returns>
	D3D12_VERTEX_BUFFER_VIEW GetView() const;

	ID3D12Resource* GetResource();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="data"></param>
	void Update(void* data);
private:

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	void* bufferMappedPtr = nullptr;

	VertexBuffer(const VertexBuffer&) = delete;

	void operator = (const VertexBuffer&) = delete;
};

