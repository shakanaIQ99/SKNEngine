#pragma once
//��XObjct3D���������N���X�����\��
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
	OBJ3D(WorldTransform* wt);

	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static OBJ3D* Create(WorldTransform* wt);

	static void SetLight(LightGroup* light) { OBJ3D::lightGroup = light; }

private:

	static ID3D12Device* device;

	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* commandList;

	static LightGroup* lightGroup;


public:

	void SetModel(ObjModel* model) { this->model = model; }

	bool Initialize();

	void Update(ViewProjection* camera);

	void Draw();

	XMFLOAT4 color;
					  
	XMFLOAT3 scale;
					  
	XMFLOAT3 rotation;
					  
	XMFLOAT3 position;

	XMMATRIX matWorld;

	WorldTransform* Wt;
private:
	ComPtr<ID3D12Resource> constBuffB1;

private:

	ObjModel* model = nullptr;



	static PipelineSet ObjPipeline;
	 


};