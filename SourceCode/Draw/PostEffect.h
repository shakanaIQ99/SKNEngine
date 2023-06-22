#pragma once
#include "Sprite2D.h"
#include <TextureManager.h>
class PostEffect :
    public Sprite2D
{
public:
    void Initialize();


    void Draw(ID3D12GraphicsCommandList* cmdlist);


private:

    TextureData* tex;

};

