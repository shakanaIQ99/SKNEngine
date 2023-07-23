#pragma once
#include <TextureManager.h>
#include "WorldTronsform.h"
#include <wrl.h>
#include <DirectXCommon.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include"TextureManager.h"
#include <Pipeline.h>

using namespace Microsoft::WRL;

class PostEffect
{
public:

    //void SetDevice();

    static void CreateGraphicsPipeline();
    static void SetDXCommon(DirectXCommon* dxcommon) { PostEffect::dxCommon = dxcommon; }

    void Initialize();

    void PreDrawScene(ID3D12GraphicsCommandList* cmdlist);

    void PostDrawScene(ID3D12GraphicsCommandList* cmdlist);

    void Draw(ID3D12GraphicsCommandList* cmdlist);

    void CreateRTV();

    void CreateDSV();

    void CreateTexBuff();

    void CreateSRV();

    void CreateDepth();

    void CreateBuffer();

    //void SetPipeline();


private:

    static const float clearcolor[4];

    struct VertexPos
    {
        XMFLOAT3 pos; // xyz座標
        XMFLOAT2 uv;
    };

    struct ConstBufferDataMaterial
    {
        XMMATRIX mat;
        XMFLOAT4 color;
    };

    static PipelineSet pipeline;
    static DirectXCommon* dxCommon;
   

    ComPtr<ID3D12Resource> TexBuff;
    ComPtr <ID3D12DescriptorHeap> srvHeap;
    D3D12_HEAP_PROPERTIES texHeapProp{};

    DescriptorHeap::DescriptorHeapViewHandle handle_;

    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

    ComPtr<ID3D12Resource> depthBuff;
    
    ComPtr<ID3D12DescriptorHeap> rtvHeap;

    ComPtr<ID3D12DescriptorHeap> dsvHeap;

    std::unique_ptr<VertexBuffer> vertexBuffer = {};

    std::unique_ptr<IndexBuffer> indexBuffer = {};

    // 定数バッファ
    ComPtr<ID3D12Resource> constBuff;

    XMFLOAT4 color = { 1,1,1,1 };

};

