#include"Object3D.h"
#include"DxWindow.h"
#include<string>
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include<fstream>
#include<sstream>
#include<vector>


using namespace std;

#pragma comment(lib, "d3dcompiler.lib")

ID3D12Device* Object3D::device = nullptr;
ViewProjection* Object3D::camera = nullptr;
PipelineSet Object3D::fbxPipeline;

//ID3D12GraphicsCommandList* Object3D::commandList;
//PipelineSet Object3D::ObjPipeline;
//LightGroup* Object3D::lightGroup = nullptr;
//
//Object3D::Object3D(WorldTransform* wt)
//{
//	Wt = wt;
//
//	color = { 1,1,1,1 };
//
//	scale = { 1,1,1 };
//
//	rotation = { 0,0,0 };
//
//	position = { 0.0f,0,0 };
//
//	matWorld = XMMatrixIdentity();
//}
//
//void Object3D::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
//{
//	assert(device);
//
//	Object3D::device = device;
//	Model::SetDevice(device);
//
//	ObjPipeline = Pipeline::CreateModelPipline(device);
//}
//
//void Object3D::PreDraw(ID3D12GraphicsCommandList* cmdList)
//{
//	commandList = cmdList;
//
//	commandList->SetPipelineState(ObjPipeline.pipelineState.Get());
//	commandList->SetGraphicsRootSignature(ObjPipeline.rootSignature.Get());
//	// プリミティブ形状の設定コマンド
//	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
//}
//
//void Object3D::PostDraw()
//{
//	Object3D::commandList = nullptr;
//}
//
//Object3D* Object3D::Create(WorldTransform* wt)
//{
//	// Spriteのインスタンスを生成
//	Object3D* obj = new Object3D(wt);
//	if (obj == nullptr) {
//		return nullptr;
//	}
//
//	// 初期化
//	if (!obj->Initialize()) {
//		delete obj;
//		assert(0);
//		return nullptr;
//	}
//
//	float scale_val = 10;
//	obj->Wt->scale_ = { scale_val ,scale_val ,scale_val };
//
//	return obj;
//}
//
//bool Object3D::Initialize()
//{
//	Wt->CreateConstBuffer(device);
//	
//	return true;
//}
//
//void Object3D::Update(ViewProjection* camera)
//{
//	
//	Wt->Map();
//
//	Wt->color = color;
//	
//	Wt->UpdateMatrix(camera->GetMAtView(), camera->GetMatProjection(),camera->Geteye());
//	
//}
//
//void Object3D::Draw()
//{
//	
//	if (model == nullptr) return;
//
//	commandList->SetGraphicsRootConstantBufferView(0, this->Wt->constBuffB0->GetGPUVirtualAddress());
//	lightGroup->Draw(commandList, 3);
//	model->Draw(commandList, 1);
//}

void Object3D::CreateGraphicsPipeline()
{
	fbxPipeline = Pipeline::CreateFBXPipeline(device);
}

void Object3D::Initilaize(WorldTransform* Wt)
{
	wt = Wt;
	wt->CreateConstBuffer(device);
}

void Object3D::Update()
{
	wt->Map();

	wt->UpdateMatrix(camera->GetMAtView(), camera->GetMatProjection(), camera->Geteye());
}

void Object3D::SetModel(Model* model)
{
	this->model = model;
	this->model->CreateBuffers(device);
}

void Object3D::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (model == nullptr)
	{
		return;
	}

	cmdList->SetPipelineState(fbxPipeline.pipelineState.Get());

	cmdList->SetGraphicsRootSignature(fbxPipeline.rootSignature.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->SetGraphicsRootConstantBufferView(0, wt->constBuffB0->GetGPUVirtualAddress());
	model->Draw(cmdList);
}
