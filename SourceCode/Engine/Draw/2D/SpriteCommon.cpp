#include "SpriteCommon.h"
#include"DxWindow.h"
#include"DirectXCommon.h"
#include<string>
#include <DirectXTex.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace SKNEngine;

ComPtr<ID3D12Device>SpriteCommon::device;
ComPtr<ID3D12GraphicsCommandList>SpriteCommon::cmdList;
PipelineSet SpriteCommon::spritepipline;


void SpriteCommon::Initialize()
{
	device = DirectXCommon::GetInstance()->GetDevice();
	cmdList = DirectXCommon::GetInstance()->GetCommandList();
	spritepipline = Pipeline::CreateSpritePipline(device.Get());
}

void SpriteCommon::PreDraw()
{
	
	cmdList->SetPipelineState(spritepipline.pipelineState.Get());
	cmdList->SetGraphicsRootSignature(spritepipline.rootSignature.Get());

}

void SpriteCommon::DrawCommand(TextureData* textureData,D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView, WorldTransform* wt)
{
	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	cmdList->IASetIndexBuffer(&ibView);
	//プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	cmdList->SetDescriptorHeaps(1, textureData->srvHeap.GetAddressOf());
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureData->gpuHandle;

	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	cmdList->SetGraphicsRootConstantBufferView(0, wt->constBuffB0->GetGPUVirtualAddress());

	// 描画コマンド
	//commandList->DrawInstanced(3, 1, 0, 0); // 全ての頂点を使って描画
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0); // 全ての頂点を使って描画

}

