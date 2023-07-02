#include "OBJ3D.h"
#include"DxWindow.h"
#include<string>
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include<fstream>
#include<sstream>
#include<vector>

using namespace std;

#pragma comment(lib, "d3dcompiler.lib")

ID3D12Device* OBJ3D::device = nullptr;
ID3D12GraphicsCommandList* OBJ3D::commandList;
PipelineSet OBJ3D::ObjPipeline;
LightGroup* OBJ3D::lightGroup = nullptr;

OBJ3D::OBJ3D(WorldTransform* wt)
{
	Wt = wt;

	color = { 1,1,1,1 };

	scale = { 1,1,1 };

	rotation = { 0,0,0 };

	position = { 0.0f,0,0 };

	matWorld = XMMatrixIdentity();
}

void OBJ3D::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	assert(device);

	OBJ3D::device = device;
	ObjModel::SetDevice(device);

	ObjPipeline = Pipeline::CreateModelPipline(device);
}

void OBJ3D::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	commandList = cmdList;

	commandList->SetPipelineState(ObjPipeline.pipelineState.Get());
	commandList->SetGraphicsRootSignature(ObjPipeline.rootSignature.Get());
	// プリミティブ形状の設定コマンド
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

void OBJ3D::PostDraw()
{
	OBJ3D::commandList = nullptr;
}

OBJ3D* OBJ3D::Create(WorldTransform* wt)
{
	OBJ3D* obj = new OBJ3D(wt);
	if (obj == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!obj->Initialize()) {
		delete obj;
		assert(0);
		return nullptr;
	}

	float scale_val = 10;
	obj->Wt->scale_ = { scale_val ,scale_val ,scale_val };

	return obj;
}

bool OBJ3D::Initialize()
{
	Wt->CreateConstBuffer(device);
	
	return true;
}

void OBJ3D::Update(ViewProjection* camera)
{
	
	Wt->Map();

	Wt->color = color;
	
	Wt->UpdateMatrix(camera->GetMAtView(), camera->GetMatProjection(),camera->Geteye());
	
}

void OBJ3D::Draw()
{
	
	if (model == nullptr) return;

	commandList->SetGraphicsRootConstantBufferView(0, this->Wt->constBuffB0->GetGPUVirtualAddress());
	lightGroup->Draw(commandList, 3);
	model->Draw(commandList, 1);
}

