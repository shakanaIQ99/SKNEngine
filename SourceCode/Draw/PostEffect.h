#pragma once
#include "Sprite2D.h"
#include <TextureManager.h>
#include "WorldTronsform.h"
#include <wrl.h>
#include <DirectXCommon.h>

using namespace Microsoft::WRL;

class PostEffect :
    public Sprite2D
{
public:

    //void SetDevice();

    void Initialize(DirectXCommon* dxcommon,SpriteCommon* spritecommon, WorldTransform* wt, uint32_t handle);


    void Draw(ID3D12GraphicsCommandList* cmdlist, XMFLOAT2 anchor = { 0.5f,0.5f }, bool flipX = false, bool flipY = false);


private:

    ComPtr<ID3D12Resource> TexBuff;
    ComPtr <ID3D12DescriptorHeap> srvHeap;
    D3D12_HEAP_PROPERTIES texHeapProp{};

    DescriptorHeap::DescriptorHeapViewHandle handle_;
};

