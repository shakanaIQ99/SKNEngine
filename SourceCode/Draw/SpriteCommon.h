#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include"DirectXCommon.h"
#include"WorldTronsform.h"
#include"Pipeline.h"
#include"TextureManager.h"

using namespace DirectX;

using namespace Microsoft::WRL;

class SpriteCommon
{

public:
    void Initialize(DirectXCommon* dxcommon);

    //void InitializeGraphicsPipeline();

    void PreDraw();

    void PostDraw();

    void DrawCommand(TextureData* textureData,D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView,WorldTransform* wt);

    DirectXCommon* GetDirextXCommon() { return dxcommon; }

    XMMATRIX Getmat() { return matProjection; }

private:

    DirectXCommon* dxcommon;

    PipelineSet spritepipline;

    ID3D12GraphicsCommandList* commandList;

    XMMATRIX matProjection{};
};