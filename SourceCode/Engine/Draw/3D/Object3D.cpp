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

ViewProjection* Object3D::camera = nullptr;
PipelineSet Object3D::fbxPipeline;


void Object3D::CreateGraphicsPipeline()
{
	fbxPipeline = Pipeline::CreateFBXPipeline(DirectXCommon::GetInstance()->GetDevice().Get());
}

void Object3D::Initilaize(WorldTransform* Wt)
{
	wt = Wt;
	wt->CreateConstBuffer();

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
	result = DirectXCommon::GetInstance()->GetDevice().Get()->CreateCommittedResource(
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
	model->CreateBuffers();
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
