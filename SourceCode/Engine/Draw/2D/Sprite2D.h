#pragma once
//2Dスプライト
#include"SpriteCommon.h"
#include"WorldTronsform.h"
#include <DirectXTex.h>
#include"TextureManager.h"
#include"IndexBuffer.h"
#include"VertexBuffer.h"
class Sprite2D
{
public:

    struct VertexPos
    {
        XMFLOAT3 pos; // xyz座標
        XMFLOAT2 uv;
    };

    struct ConstBufferDataMaterial
    {
        XMFLOAT4 color;
        XMMATRIX mat;
    };


    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="_spritecommon"></param>
    /// <param name="handle"></param>
    void Initialize(SpriteCommon* _spritecommon,uint32_t handle);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();


    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="anchor"></param>
    /// <param name="flipX"></param>
    /// <param name="flipY"></param>
    void Draw(XMFLOAT2 anchor = { 0.5f,0.5f }, bool flipX = false, bool flipY = false);

    /// <summary>
    /// 切り抜き描画
    /// </summary>
    /// <param name="ClipPos"></param>
    /// <param name="ClipSize"></param>
    /// <param name="flipX"></param>
    /// <param name="flipY"></param>
    void DrawClip(XMFLOAT2 ClipPos, XMFLOAT2 ClipSize, bool flipX = false, bool flipY = false);
    
    WorldTransform Wt;
protected:

    /// <summary>
    /// バッファ生成
    /// </summary>
    void CreateVertexIndexBuffer();

    SpriteCommon* spritecommon = nullptr;

    std::unique_ptr<VertexBuffer> vertexBuffer = {};

    std::unique_ptr<IndexBuffer> indexBuffer = {};

    shared_ptr<TextureData> tex;
   
};