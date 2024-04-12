#include "Draw3DLine.h"
#include<cassert>
#include"DirectXCommon.h"

using namespace SKNEngine;

PipelineSet Draw3DLine::pipeline;
Camera* Draw3DLine::camera = nullptr;
void Draw3DLine::Init()
{
	wt.CreateConstBuffer();
	CreateColorBuff();
	vertexBuffer = std::make_unique<VertexBuffer>();
	vertexBuffer->Create(2, sizeof(VertexPos));
}

void Draw3DLine::Draw(Vector3 startpos, Vector3 endpos)
{
	VertexPos vertices[] =
	{
		{startpos},
		{endpos},
	};

	TransferColor();
	
	wt.UpdateMatrix(camera->GetView());

	vertexBuffer->Update(vertices);

	vbView = vertexBuffer->GetView();

	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(pipeline.pipelineState.Get());
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, wt.constBuffB0->GetGPUVirtualAddress());
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	DirectXCommon::GetInstance()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
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
	result = DirectXCommon::GetInstance()->GetDevice().Get()->CreateCommittedResource(
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
