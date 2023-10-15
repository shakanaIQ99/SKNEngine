#pragma once
//インデックスバッファ
#include<wrl.h>
#include <d3d12.h>
#include <cstdint>

using namespace Microsoft::WRL;
class IndexBuffer
{
public:

    IndexBuffer() = default;
    ~IndexBuffer() = default;

    /// <summary>
    /// 生成
    /// </summary>
    /// <param name="length"></param>
    /// <param name="data"></param>
    void Create(size_t length, const void* data = nullptr);

    /// <summary>
    /// バッファビュー取得
    /// </summary>
    /// <returns></returns>
    D3D12_INDEX_BUFFER_VIEW GetView() const;

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="data"></param>
    void Update(void* data);
private:

    ComPtr<ID3D12Resource> indexBuff;

    D3D12_INDEX_BUFFER_VIEW ibView;

    void* bufferMappedPtr = nullptr;

    size_t bufferLength = 0;

    IndexBuffer(const IndexBuffer&) = delete;
    void operator = (const IndexBuffer&) = delete;
};

