#include "Draw3DLine.h"
#include<cassert>

ID3D12Device* Draw3DLine::device;
ID3D12GraphicsCommandList* Draw3DLine::cmdList;
PipelineSet Draw3DLine::pipeline;
Camera* Draw3DLine::camera = nullptr;
void Draw3DLine::Init()
{
	wt.CreateConstBuffer(device);
	CreateColorBuff();
	vertexBuffer = std::make_unique<VertexBuffer>();
	vertexBuffer->Create(device, 2, sizeof(VertexPos));
}

void Draw3DLine::Draw(Vector3 startpos, Vector3 endpos)
{
	VertexPos vertices[] =
	{
		{startpos},
		{endpos},
	};

	TransferColor();
	
	wt.UpdateMatrix(camera->getView());

	vertexBuffer->Update(vertices);

	vbView = vertexBuffer->GetView();

	cmdList->SetPipelineState(pipeline.pipelineState.Get());
	cmdList->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	cmdList->SetGraphicsRootConstantBufferView(0, wt.constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
}

void Draw3DLine::SetDevice(ID3D12Device* _device, ID3D12GraphicsCommandList* _cmdList)
{
	device = _device;
	cmdList = _cmdList;
	CreateGraphicsPipeline();
}

void Draw3DLine::CreateColorBuff()
{
	HRESULT result = S_FALSE;

	D3D12_HEAP_PROPERTIES cbHeapPropB1{};

	cbHeapPropB1.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC cbResourceDescB1{};
	//リソース設定
	cbResourceDescB1.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDescB1.Width = (sizeof(Color) + 0xff) & ~0Xff;	//256バイトアライメント
	cbResourceDescB1.Height = 1;
	cbResourceDescB1.DepthOrArraySize = 1;
	cbResourceDescB1.MipLevels = 1;
	cbResourceDescB1.SampleDesc.Count = 1;
	cbResourceDescB1.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapPropB1,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDescB1,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1)
	);
	assert(SUCCEEDED(result));
}

void Draw3DLine::TransferColor()
{
	HRESULT result = S_FALSE;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->color = color;
	constBuffB1->Unmap(0, nullptr);
}
