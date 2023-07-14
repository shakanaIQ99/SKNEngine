#pragma once
#include "Sprite2D.h"
#include <TextureManager.h>
#include "WorldTronsform.h"
class PostEffect :
    public Sprite2D
{
public:
    void Initialize(SpriteCommon* spritecommon, WorldTransform* wt, uint32_t handle);


    void Draw(ID3D12GraphicsCommandList* cmdlist, XMFLOAT2 anchor = { 0.5f,0.5f }, bool flipX = false, bool flipY = false);


private:


};

