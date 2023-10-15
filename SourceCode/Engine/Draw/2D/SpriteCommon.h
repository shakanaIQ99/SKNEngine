#pragma once
//汎用スプライト
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
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    //void InitializeGraphicsPipeline();

    /// <summary>
    /// 描画前処理
    /// </summary>
    void PreDraw();

    /// <summary>
    /// 描画コマンド
    /// </summary>
    /// <param name="textureData"></param>
    /// <param name="vbView"></param>
    /// <param name="ibView"></param>
    /// <param name="wt"></param>
    void DrawCommand(TextureData* textureData,D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView,WorldTransform* wt);

    /// <summary>
    /// 射影行列取得
    /// </summary>
    /// <returns></returns>
    XMMATRIX Getmat() { return matProjection; }

private:
    PipelineSet spritepipline;

    XMMATRIX matProjection{};
};