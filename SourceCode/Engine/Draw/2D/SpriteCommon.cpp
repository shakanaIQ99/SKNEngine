#include "SpriteCommon.h"
#include"DxWindow.h"
#include<string>
#include <DirectXTex.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

void SpriteCommon::Initialize(DirectXCommon* _dxcommon)
{
	this->dxcommon = _dxcommon;

	spritepipline = Pipeline::CreateSpritePipline(dxcommon->GetDevice());

	matProjection = XMMatrixOrthographicOffCenterLH(0.0f, DxWindow::window_width, DxWindow::window_height, 0.0f, 0.0f, 1.0f);

}

void SpriteCommon::PreDraw()
{
	commandList = dxcommon->GetCommandList();

	dxcommon->GetCommandList()->SetPipelineState(spritepipline.pipelineState.Get());
	dxcommon->GetCommandList()->SetGraphicsRootSignature(spritepipline.rootSignature.Get());

}

void SpriteCommon::PostDraw()
{
	commandList = nullptr;
}

void SpriteCommon::DrawCommand(TextureData* textureData,D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView, WorldTransform* wt)
{
	// 頂点バッファビューの設定コマンド
	dxcommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	dxcommon->GetCommandList()->IASetIndexBuffer(&ibView);
	//プリミティブ形状の設定コマンド
	dxcommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	dxcommon->GetCommandList()->SetDescriptorHeaps(1, textureData->srvHeap.GetAddressOf());
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureData->gpuHandle;

	dxcommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	dxcommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, wt->constBuffB0->GetGPUVirtualAddress());

	// 描画コマンド
	//commandList->DrawInstanced(3, 1, 0, 0); // 全ての頂点を使って描画
	dxcommon->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0); // 全ての頂点を使って描画

}

