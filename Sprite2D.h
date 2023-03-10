#pragma once
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
        XMFLOAT3 pos; // xyz???W
        XMFLOAT2 uv;
    };

    struct ConstBufferDataMaterial
    {
        XMFLOAT4 color;
        XMMATRIX mat;
    };



    void Initialize(SpriteCommon* spritecommon, WorldTransform* wt, uint32_t handle);

    void Update();

    void Draw(XMFLOAT2 anchor = { 0.5f,0.5f }, bool flipX = false, bool flipY = false);

    void DrawClip(XMFLOAT2 ClipPos, XMFLOAT2 ClipSize, bool flipX = false, bool flipY = false);
    
    WorldTransform* Wt = nullptr;
private:

    void CreateVertexIndexBuffer();

    SpriteCommon* spritecommon = nullptr;
    ID3D12Device* device = nullptr;

    std::unique_ptr<VertexBuffer> vertexBuffer = {};

    std::unique_ptr<IndexBuffer> indexBuffer = {};

    TextureData* tex;
   
};