#include"Object3D.h"
#include"DxWindow.h"
#include<string>
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include<fstream>
#include<sstream>
#include<vector>

#include <FbxLoader.h>


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

	HRESULT result;

	D3D12_HEAP_PROPERTIES cbHeapPropB1{};

	cbHeapPropB1.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC cbResourceDescB1{};
	//リソース設定
	cbResourceDescB1.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDescB1.Width = (sizeof(ConstBufferDataSkin) + 0xff) & ~0Xff;	//256バイトアライメント
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
		IID_PPV_ARGS(&constBuffSkin)
	);

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin->Unmap(0, nullptr);
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void Object3D::Update()
{

	HRESULT result;


	wt->UpdateMatrix(camera);

	vector<Model::Bone>& bones = model->GetBones();

	ConstBufferDataSkin* constMapSkin = nullptr;
	
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);

	if (isPlay)
	{
		currentTime += frameTime;
		if (currentTime > endTime)
		{
			currentTime = startTime;
		}
	}

	for (size_t i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose;

		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

}

void Object3D::SetModel(Model* _model)
{
	model = _model;
	model->CreateBuffers(device);
}

void Object3D::PlayAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();

	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);

	const char* animstackname = animstack->GetName();

	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	startTime = takeinfo->mLocalTimeSpan.GetStart();

	endTime = takeinfo->mLocalTimeSpan.GetStop();

	currentTime = startTime;

	isPlay = true;

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
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());
	model->Draw(cmdList);
}
