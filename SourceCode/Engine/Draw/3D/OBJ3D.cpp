#include "OBJ3D.h"
#include"DxWindow.h"
#include "DirectXCommon.h"
#include<string>
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include<fstream>
#include<sstream>
#include<vector>

using namespace std;
using namespace SKNEngine;

#pragma comment(lib, "d3dcompiler.lib")
PipelineSet OBJ3D::ObjPipeline;
LightGroup* OBJ3D::lightGroup = nullptr;

OBJ3D::OBJ3D()
{

	color = { 1,1,1,1 };

	scale = { 1,1,1 };

	rotation = { 0,0,0 };

	position = { 0.0f,0,0 };

	matWorld =Matrix4();
}

void OBJ3D::StaticInitialize()
{
	
	ObjModel::SetDevice(DirectXCommon::GetDevice().Get());

	ObjPipeline = Pipeline::CreateModelPipline(DirectXCommon::GetDevice().Get());
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
	DirectXCommon::GetCommandList()->SetPipelineState(ObjPipeline.pipelineState.Get());
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(ObjPipeline.rootSignature.Get());
	// プリミティブ形状の設定コマンド
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
	
	if (model == nullptr) return;

	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, Wt.constBuffB0->GetGPUVirtualAddress());
	lightGroup->Draw(DirectXCommon::GetCommandList(), 3);
	model->Draw(DirectXCommon::GetCommandList(), 1);
}

