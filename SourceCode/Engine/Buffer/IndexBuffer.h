#pragma once
#include<wrl.h>
#include <d3d12.h>
#include <cstdint>

using namespace Microsoft::WRL;
class IndexBuffer
{
public:

    IndexBuffer() = default;
    ~IndexBuffer() = default;

    void Create(size_t length, const void* data = nullptr);

    D3D12_INDEX_BUFFER_VIEW GetView() const;

    void Update(void* data);
private:

    ComPtr<ID3D12Resource> indexBuff;

    D3D12_INDEX_BUFFER_VIEW ibView;

    void* bufferMappedPtr = nullptr;

    size_t bufferLength = 0;

    IndexBuffer(const IndexBuffer&) = delete;
    void operator = (const IndexBuffer&) = delete;
};

