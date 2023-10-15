#pragma once
//ポストエフェクト
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

    /// <summary>
    /// パイプライン生成
    /// </summary>
    static void CreateGraphicsPipeline();

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 描画前処理
    /// </summary>
    void PreDrawScene();

    /// <summary>
    /// 描画後処理
    /// </summary>
    void PostDrawScene();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    /// <summary>
    /// RTV生成
    /// </summary>
    void CreateRTV();

    /// <summary>
    /// DSV生成
    /// </summary>
    void CreateDSV();

    /// <summary>
    /// テクスチャバッファ生成
    /// </summary>
    void CreateTexBuff();

    /// <summary>
    /// SRV生成
    /// </summary>
    void CreateSRV();

    /// <summary>
    /// 深度生成
    /// </summary>
    void CreateDepth();

    /// <summary>
    /// バッファ生成
    /// </summary>
    void CreateBuffer();

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

