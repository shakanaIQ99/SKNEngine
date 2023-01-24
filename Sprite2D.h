#pragma once
#include"SpriteCommon.h"
#include"WorldTronsform.h"
#include <DirectXTex.h>
class Sprite2D
{
public:

    struct VertexPos
    {
        XMFLOAT3 pos; // xyzç¿ïW
        XMFLOAT2 uv;
    };

    struct ConstBufferDataMaterial
    {
        XMFLOAT4 color;
        XMMATRIX mat;
    };



    void Initialize(SpriteCommon* spritecommon, WorldTransform* wt);

    void Update();

    void Draw();

    
    WorldTransform* Wt = nullptr;
private:
    SpriteCommon* spritecommon = nullptr;
    ID3D12Device* device = nullptr;

    ComPtr<ID3D12Resource> vertBuff;

    ComPtr<ID3D12Resource> indexBuff;

    

    D3D12_VERTEX_BUFFER_VIEW vbView;
    D3D12_INDEX_BUFFER_VIEW ibView;
   
};