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



OBJ3D::OBJ3D()
{

	color = { 1,1,1,1 };

	scale = { 1,1,1 };

	rotation = { 0,0,0 };

	position = { 0.0f,0,0 };

	matWorld = XMMatrixIdentity();
}

void OBJ3D::StaticInitialize(ID3D12Device* _device)
{
	assert(_device);

	OBJ3D::device = _device;
	ObjModel::SetDevice(device);

	ObjPipeline = Pipeline::CreateModelPipline(device);
}

void OBJ3D::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	commandList = cmdList;

	
}


OBJ3D* OBJ3D::Create()
{
	OBJ3D* obj = new OBJ3D();
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
	obj->Wt.scale_ = { scale_val ,scale_val ,scale_val };

	return obj;
}

bool OBJ3D::Initialize()
{
	Wt.CreateConstBuffer(device);
	
	return true;
}

void OBJ3D::Update(ViewProjection* camera)
{
	

	Wt.color = color;
	
	Wt.UpdateMatrix(camera);
	
}

void OBJ3D::Draw()
{
	commandList->SetPipelineState(ObjPipeline.pipelineState.Get());
	commandList->SetGraphicsRootSignature(ObjPipeline.rootSignature.Get());
	// プリミティブ形状の設定コマンド
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
	
	if (model == nullptr) return;

	commandList->SetGraphicsRootConstantBufferView(0, Wt.constBuffB0->GetGPUVirtualAddress());
	lightGroup->Draw(commandList, 3);
	model->Draw(commandList, 1);
}

