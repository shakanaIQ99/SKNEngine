#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include"DirectXCommon.h"
#include"WorldTronsform.h"
#include"Pipeline.h"
#include"TextureManager.h"

using namespace DirectX;

using namespace Microsoft::WRL;

class SpriteCommon
{

public:

    static ComPtr<ID3D12Device>device;
    static ComPtr<ID3D12GraphicsCommandList>cmdList;

    static void Initialize();

    static void PreDraw();
    static void DrawCommand(TextureHandle Handle,D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView,WorldTransform* wt);


private:

    static PipelineSet spritepipline;
};