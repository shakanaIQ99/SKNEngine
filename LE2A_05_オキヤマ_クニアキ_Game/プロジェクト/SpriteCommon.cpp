#include "SpriteCommon.h"
#include"DxWindow.h"
#include<string>
#include <DirectXTex.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

void SpriteCommon::Initialize(DirectXCommon* dxcommon)
{
	this->dxcommon = dxcommon;

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
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	dxcommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	dxcommon->GetCommandList()->IASetIndexBuffer(&ibView);
	//�v���~�e�B�u�`��̐ݒ�R�}���h
	dxcommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	dxcommon->GetCommandList()->SetDescriptorHeaps(1, textureData->srvHeap.GetAddressOf());
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureData->gpuHandle;

	dxcommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	dxcommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, wt->constBuffB0->GetGPUVirtualAddress());

	// �`��R�}���h
	//commandList->DrawInstanced(3, 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
	dxcommon->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��

}

