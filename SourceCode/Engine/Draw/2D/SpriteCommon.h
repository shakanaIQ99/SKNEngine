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
    void Initialize();

    //void InitializeGraphicsPipeline();

    void PreDraw();

    void DrawCommand(TextureData* textureData,D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView,WorldTransform* wt);

    XMMATRIX Getmat() { return matProjection; }

private:
    PipelineSet spritepipline;

    XMMATRIX matProjection{};
};