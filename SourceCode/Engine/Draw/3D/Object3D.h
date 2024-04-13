//#pragma once
//#include <Windows.h>
//#include <wrl.h>
//#include <d3d12.h>
//#include"WorldTronsform.h"
//#include<vector>
//#include<string>
//#include"Model.h"
//#include"Pipeline.h"
//#include"ViewProjection.h"
//#include"LightGroup.h"
//#include"Matrix4.h"
//
//
//using namespace DirectX;
//
//using namespace Microsoft::WRL;
//
//using namespace std;
//
//
//class Object3D
//{
//public:
//
//	static const int MAX_BONES = 32;
//
//	ComPtr<ID3D12Resource> constBuffSkin;
//
//	struct ConstBufferDataSkin
//	{
//		Matrix4 bones[MAX_BONES];
//	};
//
//	static void SetDevice(ID3D12Device* Device) { Object3D::device = Device; }
//	static void SetCamera(ViewProjection* Camera) { Object3D::camera = Camera; }
//	static void CreateGraphicsPipeline();
//
//	void Initilaize(WorldTransform* Wt);
//
//	void Update();
//
//	void Draw(ID3D12GraphicsCommandList* cmdList);
//
//	void SetModel(Model* model);
//
//	void PlayAnimation();
//
//private:
//
//	static ID3D12Device* device;
//
//	static ViewProjection* camera;
//
//	static PipelineSet fbxPipeline;
//
//	FbxTime frameTime;
//
//	FbxTime startTime;
//
//	FbxTime endTime;
//
//	FbxTime currentTime;
//
//	bool isPlay = false;
//protected:
//	WorldTransform* wt = nullptr;
//
//	Model* model = nullptr;
//
//};
//
//
//
////class Object3D
////{
////
////
////
////public:
////
////	Object3D(WorldTransform* wt);
////
////	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);
////
////	/// <param name="cmdList">描画コマンドリスト</param>
////	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
////
////	/// <summary>
////	/// 描画後処理
////	/// </summary>
////	static void PostDraw();
////
////	static Object3D* Create(WorldTransform* wt);
////
////	static void SetLight(LightGroup* light) { Object3D::lightGroup = light; }
////
////private:
////
////	static ID3D12Device* device;
////
////	// コマンドリスト
////	static ID3D12GraphicsCommandList* commandList;
////
////	static LightGroup* lightGroup;
////
////
////public:
////
////	void SetModel(Model* model) { this->model = model; }
////
////	bool Initialize();
////
////	void Update(ViewProjection* camera);
////
////	void Draw();
////
////	XMFLOAT4 color;
////					  
////	XMFLOAT3 scale;
////					  
////	XMFLOAT3 rotation;
////					  
////	XMFLOAT3 position;
////
////	XMMATRIX matWorld;
////
////	WorldTransform* Wt;
////private:
////	ComPtr<ID3D12Resource> constBuffB1;
////
////private:
////
////	Model* model = nullptr;
////
////
////
////	static PipelineSet ObjPipeline;
////	 
////
////
////};
////
