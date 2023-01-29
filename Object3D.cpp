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
ID3D12GraphicsCommandList* Object3D::commandList;
XMMATRIX Object3D::matView{};
XMMATRIX Object3D::matProjection{};
XMFLOAT3 Object3D::eye = { 0, 20.0f, -50.0f };
XMFLOAT3 Object3D::target = { 0,0,0 };
XMFLOAT3 Object3D::up = { 0,1,0 };
PipelineSet Object3D::ObjPipeline;

Object3D::Object3D(WorldTransform* wt)
{
	Wt = wt;

	color = { 1,1,1,1 };

	scale = { 1,1,1 };

	rotation = { 0,0,0 };

	position = { 0.0f,0,0 };

	matWorld = XMMatrixIdentity();
}

void Object3D::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	assert(device);

	Object3D::device = device;
	Model::SetDevice(device);
	
	InitializeCamera();

	ObjPipeline = Pipeline::CreateModelPipline(device);
}

void Object3D::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	commandList = cmdList;

	commandList->SetPipelineState(ObjPipeline.pipelineState.Get());
	commandList->SetGraphicsRootSignature(ObjPipeline.rootSignature.Get());
	// プリミティブ形状の設定コマンド
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

void Object3D::PostDraw()
{
	Object3D::commandList = nullptr;
}

Object3D* Object3D::Create(WorldTransform* wt)
{
	// Spriteのインスタンスを生成
	Object3D* obj = new Object3D(wt);
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

bool Object3D::Initialize()
{
	Wt->CreateConstBuffer(device);
	
	return true;
}

void Object3D::InitializeCamera()
{
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	matProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)DxWindow::window_width / DxWindow::window_height, 0.1f, 1000.0f);

}



void Object3D::Update()
{
	
	Wt->Map();
	
	Wt->UpdateMatrix(matView, matProjection);
	
}

void Object3D::UpdateViewMat()
{
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}


void Object3D::Draw()
{
	
	if (model == nullptr) return;

	commandList->SetGraphicsRootConstantBufferView(0, this->Wt->constBuffB0->GetGPUVirtualAddress());
	model->Draw(commandList, 1);
}
