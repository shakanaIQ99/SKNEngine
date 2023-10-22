#pragma once
//後々Objct3Dを改造しクラス統合予定
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include"WorldTronsform.h"
#include<vector>
#include<string>
#include"ObjModel.h"
#include"Pipeline.h"
#include"ViewProjection.h"
#include"LightGroup.h"


using namespace DirectX;

using namespace Microsoft::WRL;

using namespace std;


class OBJ3D
{



public:

	OBJ3D();


	static void StaticInitialize(ID3D12Device* device);

	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static OBJ3D* Create();

	static void SetLight(LightGroup* light) { OBJ3D::lightGroup = light; }

private:

	static ID3D12Device* device;

	// コマンドリスト
	static ID3D12GraphicsCommandList* commandList;

	static LightGroup* lightGroup;


public:

	void SetModel(ObjModel* _model) { model = _model; }

	bool Initialize();

	void Update(ViewProjection* camera);

	void Draw();

	XMFLOAT4 color;
					  
	XMFLOAT3 scale;
					  
	XMFLOAT3 rotation;
					  
	XMFLOAT3 position;

	XMMATRIX matWorld;

	WorldTransform Wt;
private:
	ComPtr<ID3D12Resource> constBuffB1;

private:

	ObjModel* model = nullptr;



	static PipelineSet ObjPipeline;
	 


};
