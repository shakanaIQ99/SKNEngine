#pragma once
#include<wrl.h>
#include <d3d12.h>

using namespace Microsoft::WRL;

class VertexBuffer
{
public:

	VertexBuffer() = default;
	~VertexBuffer() = default;

	void Create(size_t length, size_t singleSize, const void* data = nullptr);

	D3D12_VERTEX_BUFFER_VIEW GetView() const;

	ID3D12Resource* GetResource();

	void Update(void* data);
private:

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	void* bufferMappedPtr = nullptr;

	VertexBuffer(const VertexBuffer&) = delete;

	void operator = (const VertexBuffer&) = delete;
};

