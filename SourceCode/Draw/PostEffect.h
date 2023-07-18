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

    void Initialize(DirectXCommon* dxcommon,SpriteCommon* spritecommon, WorldTransform* wt);

    void PreDrawScene(ID3D12GraphicsCommandList* cmdlist);

    void PostDrawScene(ID3D12GraphicsCommandList* cmdlist);

    void Draw(ID3D12GraphicsCommandList* cmdlist, XMFLOAT2 anchor = { 0.5f,0.5f }, bool flipX = false, bool flipY = false);

    void CreateRTV();

    void CreateDSV();

    void CreateTexBuff();

    void CreateSRV();

    void CreateDepth();


private:

    static const float clearcolor[4];

    DirectXCommon* dxCommon;

    ComPtr<ID3D12Resource> TexBuff;
    ComPtr <ID3D12DescriptorHeap> srvHeap;
    D3D12_HEAP_PROPERTIES texHeapProp{};

    DescriptorHeap::DescriptorHeapViewHandle handle_;

    ComPtr<ID3D12Resource> depthBuff;
    
    ComPtr<ID3D12DescriptorHeap> rtvHeap;

    ComPtr<ID3D12DescriptorHeap> dsvHeap;
};

