#pragma once
#include"WorldTronsform.h"
#include <DirectXTex.h>
#include"TextureManager.h"
#include"IndexBuffer.h"
#include"VertexBuffer.h"
#include"Vector2.h"
class Sprite2D
{
public:

    struct VertexPos
    {
        Vector3 pos; // xyz座標
        Vector2 uv;
    };

    struct ConstBufferDataMaterial
    {
        Float4 color;
        Matrix4 mat;
    };



    void Initialize(TextureHandle Handle);

    void Update();

    void Draw(float x, float y, float x2, float y2, bool flipX = false, bool flipY = false);

    void Draw(Vector2 anchor = { 0.5f,0.5f }, bool flipX = false, bool flipY = false);

    void DrawClip(Vector2 ClipPos, Vector2 ClipSize, bool flipX = false, bool flipY = false);

    void SetTexture(TextureHandle Handle);
    
    WorldTransform Wt;
protected:

    void CreateVertexIndexBuffer();

    Vector2 size = { 0, 0 };

    std::unique_ptr<VertexBuffer> vertexBuffer = {};

    std::unique_ptr<IndexBuffer> indexBuffer = {};

    Matrix4 matProjection;
   
    TextureHandle handle;
};