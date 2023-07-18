#include "PostEffect.h"
#include <d3dx12.h>
#include "DxWindow.h"

using namespace DirectX;

void PostEffect::Initialize(DirectXCommon* dxcommon,SpriteCommon* spritecommon, WorldTransform* wt, uint32_t handle)
{
	HRESULT result;

	Sprite2D::Initialize(spritecommon, wt, handle);
	Update();

	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC rsDesc{};
	rsDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rsDesc.Format =DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rsDesc.Width = DxWindow::window_width;
	rsDesc.Height = DxWindow::window_height;
	rsDesc.DepthOrArraySize =1;
	rsDesc.MipLevels = 0;
	rsDesc.SampleDesc.Count = 1;
	rsDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	result = dxcommon->GetDevice()->CreateCommittedResource(
		&texHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&rsDesc,	//リソース設定
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(TexBuff.ReleaseAndGetAddressOf())
	);

	const UINT pixelCount = DxWindow::window_width * DxWindow::window_height;

	const UINT rowPitch = sizeof(UINT) * DxWindow::window_width;

	const UINT depthPitch = rowPitch + DxWindow::window_height;

	UINT* img = new UINT[pixelCount];
	for (size_t i = 0; i < pixelCount; i++)
	{
		img[i] = 0xff0000ff;
	}

	result = TexBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
	assert(SUCCEEDED(result));
	delete[] img;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels =1;

	srvHeap = dxcommon->GetDescriptorHeap()->GetHeap();

	handle_ = dxcommon->GetDescriptorHeap()->AddSRV();

	dxcommon->GetDevice()->CreateShaderResourceView(TexBuff.Get(), &srvDesc, handle_.cpuHandle);


}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdlist, XMFLOAT2 anchor, bool flipX, bool flipY)
{
	int isFlipX, isFlipY;
	if (flipX == false)isFlipX = 1;
	else isFlipX = -1;
	if (flipY == false)isFlipY = 1;
	else isFlipY = -1;

	float left = ((0.0f - anchor.x) * tex->width) * isFlipX;
	float right = ((1.0f - anchor.x) * tex->width) * isFlipX;
	float top = ((0.0f - anchor.y) * tex->height) * isFlipY;
	float bottom = ((1.0f - anchor.y) * tex->height) * isFlipY;

	VertexPos vertices[] =
	{
		{{left,top,0.0f},{0.0f,0.0f}	},
		{{left,bottom,0.0f},{0.0f,1.0f}	},
		{{right,top,0.0f},{1.0f,0.0f}	},
		{{right,bottom,0.0f},{1.0f,1.0f}}
	};
	uint32_t indices[] =
	{
		1,0,3,
		2,3,0,
	};

	vertexBuffer->Update(vertices);

	indexBuffer->Update(indices);

	Update();

	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();

	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	//spritecommon->DrawCommand(tex, vertexBuffer->GetView(), indexBuffer->GetView(), Wt);

	// 頂点バッファビューの設定コマンド
	cmdlist->IASetVertexBuffers(0, 1, &vbView);

	cmdlist->IASetIndexBuffer(&ibView);
	//プリミティブ形状の設定コマンド
	cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	cmdlist->SetDescriptorHeaps(1, srvHeap.GetAddressOf());
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = handle_.gpuHandle;

	cmdlist->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	cmdlist->SetGraphicsRootConstantBufferView(0, Wt->constBuffB0->GetGPUVirtualAddress());

	// 描画コマンド
	//commandList->DrawInstanced(3, 1, 0, 0); // 全ての頂点を使って描画
	cmdlist->DrawIndexedInstanced(6, 1, 0, 0, 0); // 全ての頂点を使って描画
}
