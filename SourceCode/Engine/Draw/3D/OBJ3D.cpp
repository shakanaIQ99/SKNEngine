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

void OBJ3D::StaticInitialize()
{
	ObjPipeline = Pipeline::CreateModelPipline(DirectXCommon::GetInstance()->GetDevice().Get());
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
	Wt.CreateConstBuffer();
	
	return true;
}

void OBJ3D::Update(ViewProjection* camera)
{
	

	Wt.color = color;
	
	Wt.UpdateMatrix(camera);
	
}

void OBJ3D::Draw()
{


	DirectXCommon::GetInstance()->GetCommandList().Get()->SetPipelineState(ObjPipeline.pipelineState.Get());
	DirectXCommon::GetInstance()->GetCommandList().Get()->SetGraphicsRootSignature(ObjPipeline.rootSignature.Get());
	// プリミティブ形状の設定コマンド
	DirectXCommon::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
	
	if (model == nullptr) return;

	DirectXCommon::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0, Wt.constBuffB0->GetGPUVirtualAddress());
	lightGroup->Draw(3);
	model->Draw(1);
}

