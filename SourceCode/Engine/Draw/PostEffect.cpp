#include "PostEffect.h"
#include <d3dx12.h>
#include "DxWindow.h"
#include<cassert>


using namespace DirectX;

const float PostEffect::clearcolor[4] = { 0.25f,0.25f,0.25f,0.0f };
PipelineSet PostEffect::pipeline;
DirectXCommon* PostEffect::dxCommon = nullptr;

void PostEffect::CreateGraphicsPipeline()
{
	pipeline = Pipeline::CreatePostEffectPipeline(dxCommon->GetDevice());
}

void PostEffect::Initialize()
{
	//Sprite2D::Initialize(spritecommon, wt, a);

	CreateBuffer();

	//Wt->CreateConstBuffer(dxCommon->GetDevice());

	CreateTexBuff();

	CreateSRV();

	CreateRTV();
	
	CreateDepth();
	
	CreateDSV();

	


}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdlist)
{
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(TexBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	cmdlist->ResourceBarrier(1, &barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	cmdlist->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	auto viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, DxWindow::window_width, DxWindow::window_height);

	cmdlist->RSSetViewports(1, &viewport);

	auto scissorRects = CD3DX12_RECT(0, 0, DxWindow::window_width, DxWindow::window_height);

	cmdlist->RSSetScissorRects(1, &scissorRects);

	cmdlist->ClearRenderTargetView(rtvH, clearcolor, 0, nullptr);

	cmdlist->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdlist)
{
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(TexBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	cmdlist->ResourceBarrier(1, &barrier);
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdlist)
{
	
	VertexPos vertices[] =
	{
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f}	},
		{{-1.0f,+1.0f,0.0f},{0.0f,0.0f}	},
		{{+1.0f,-1.0f,0.0f},{1.0f,1.0f}	},
		{{+1.0f,+1.0f,0.0f},{1.0f,0.0f}}
	};
	
	uint32_t indices[] =
	{
		1,0,3,
		2,3,0,
	};

	vertexBuffer->Update(vertices);

	indexBuffer->Update(indices);

	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();

	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	cmdlist->SetPipelineState(pipeline.pipelineState.Get());
	cmdlist->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	//spritecommon->DrawCommand(tex, vertexBuffer->GetView(), indexBuffer->GetView(), Wt);

	// 頂点バッファビューの設定コマンド
	cmdlist->IASetVertexBuffers(0, 1, &vbView);

	cmdlist->IASetIndexBuffer(&ibView);
	//プリミティブ形状の設定コマンド
	cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	//cmdlist->SetDescriptorHeaps(1, dxCommon->GetDescriptorHeap()->GetHeap().GetAddressOf());
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = gpuHandle;

	cmdlist->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	cmdlist->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	// 描画コマンド
	cmdlist->DrawIndexedInstanced(6, 1, 0, 0, 0); // 全ての頂点を使って描画
}

void PostEffect::CreateRTV()
{

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};

	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	HRESULT result;
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&rtvHeap));
	assert(SUCCEEDED(result));

	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	dxCommon->GetDevice()->CreateRenderTargetView(TexBuff.Get(), &renderTargetViewDesc, rtvHeap->GetCPUDescriptorHandleForHeapStart());

}

void PostEffect::CreateDSV()
{
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;

	HRESULT result;

	result = dxCommon->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&dsvHeap));

	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dxCommon->GetDevice()->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());


}

void PostEffect::CreateTexBuff()
{
	HRESULT result;
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC rsDesc{};
	rsDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rsDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rsDesc.Width = DxWindow::window_width;
	rsDesc.Height = DxWindow::window_height;
	rsDesc.DepthOrArraySize = 1;
	rsDesc.MipLevels = 0;
	rsDesc.SampleDesc.Count = 1;
	rsDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearcolor);

	result = dxCommon->GetDevice()->CreateCommittedResource(
		&texHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&rsDesc,	//リソース設定
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
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
}

void PostEffect::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	gpuHandle.ptr = dxCommon->GetDescriptorHeap()->CreateSRV(srvDesc, TexBuff.Get());
}

void PostEffect::CreateDepth()
{
	D3D12_RESOURCE_DESC depthDesc{};
	depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.Width = DxWindow::window_width;
	depthDesc.Height = DxWindow::window_height;
	depthDesc.DepthOrArraySize = 1;
	depthDesc.MipLevels = 0;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	HRESULT result;

	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&depthDesc,	//リソース設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&depthBuff)
	);

	assert(SUCCEEDED(result));

}

void PostEffect::CreateBuffer()
{
	vertexBuffer = make_unique<VertexBuffer>();
	vertexBuffer->Create(dxCommon->GetDevice(), 4, sizeof(VertexPos));

	indexBuffer = make_unique<IndexBuffer>();
	indexBuffer->Create(dxCommon->GetDevice(), 6);
	HRESULT result;

	D3D12_HEAP_PROPERTIES cbHeapProp{};

	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	auto rsDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff);

	//定数バッファの生成
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&rsDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	assert(SUCCEEDED(result));

	ConstBufferDataMaterial* constMap = nullptr;

	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = this->color;
		constMap->mat = XMMatrixIdentity();
		constBuff->Unmap(0, nullptr);
	}
	
}


