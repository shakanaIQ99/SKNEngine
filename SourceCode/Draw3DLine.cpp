#include "Draw3DLine.h"

ID3D12Device* Draw3DLine::device;
ID3D12GraphicsCommandList* Draw3DLine::cmdList;
PipelineSet Draw3DLine::pipeline;
Camera* Draw3DLine::camera = nullptr;
void Draw3DLine::Init()
{
	wt.CreateConstBuffer(device);

	vertexBuffer = std::make_unique<VertexBuffer>();
	vertexBuffer->Create(device, 2, sizeof(VertexPos));
}

void Draw3DLine::Draw(XMFLOAT3 startpos, XMFLOAT3 endpos)
{
	VertexPos vertices[] =
	{
		{startpos},
		{endpos},
	};

	wt.color = color;

	wt.UpdateMatrix(camera->getView());

	vertexBuffer->Update(vertices);

	vbView = vertexBuffer->GetView();

	cmdList->SetPipelineState(pipeline.pipelineState.Get());
	cmdList->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	cmdList->SetGraphicsRootConstantBufferView(0, wt.constBuffB0->GetGPUVirtualAddress());

	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
}
