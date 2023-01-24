#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include"DirectXCommon.h"
#include"WorldTronsform.h"
#include"Pipeline.h"

using namespace DirectX;

using namespace Microsoft::WRL;

class SpriteCommon
{

public:
    void Initialize(DirectXCommon* dxcommon);

    void InitializeGraphicsPipeline();

    void CreateDescriptorHeap();

    void PreDraw();

    void PostDraw();

    void DrawCommand(D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView,WorldTransform* wt);

    void LoadTexture(UINT texnumber, const wchar_t* filename);

    DirectXCommon* GetDirextXCommon() { return dxcommon; }

    XMMATRIX Getmat() { return matProjection; }

    ID3D12DescriptorHeap* GetDescHeap() { return descHeap.Get(); }

private:

    static const int SrvCount = 512;

    ComPtr<ID3D12Resource> texBuff[SrvCount];

    DirectXCommon* dxcommon;

    PipelineSet spritepipline;

    ID3D12GraphicsCommandList* commandList;

    ComPtr<ID3D12DescriptorHeap> descHeap;

    ComPtr<ID3D12RootSignature> rootsignature;
    // パイプラインステートオブジェクト
    ComPtr<ID3D12PipelineState> pipelinestate;

    D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

    XMMATRIX matProjection{};
};