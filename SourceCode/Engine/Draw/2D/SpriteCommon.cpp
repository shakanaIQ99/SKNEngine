#include "SpriteCommon.h"
#include"DxWindow.h"
#include<string>
#include <DirectXTex.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

void SpriteCommon::Initialize()
{
	spritepipline = Pipeline::CreateSpritePipline(DirectXCommon::GetInstance()->GetDevice().Get());

	matProjection = XMMatrixOrthographicOffCenterLH(0.0f, DxWindow::window_width, DxWindow::window_height, 0.0f, 0.0f, 1.0f);

}

void SpriteCommon::PreDraw()
{

	DirectXCommon::GetInstance()->GetCommandList().Get()->SetPipelineState(spritepipline.pipelineState.Get());
	DirectXCommon::GetInstance()->GetCommandList().Get()->SetGraphicsRootSignature(spritepipline.rootSignature.Get());

}

void SpriteCommon::DrawCommand(TextureData* textureData,D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView, WorldTransform* wt)
{
	// 頂点バッファビューの設定コマンド
	DirectXCommon::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &vbView);

	DirectXCommon::GetInstance()->GetCommandList().Get()->IASetIndexBuffer(&ibView);
	//プリミティブ形状の設定コマンド
	DirectXCommon::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	DirectXCommon::GetInstance()->GetCommandList().Get()->SetDescriptorHeaps(1, textureData->srvHeap.GetAddressOf());
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureData->gpuHandle;

	DirectXCommon::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	DirectXCommon::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0, wt->constBuffB0->GetGPUVirtualAddress());

	// 描画コマンド
	//commandList->DrawInstanced(3, 1, 0, 0); // 全ての頂点を使って描画
	DirectXCommon::GetInstance()->GetCommandList().Get()->DrawIndexedInstanced(6, 1, 0, 0, 0); // 全ての頂点を使って描画

}

