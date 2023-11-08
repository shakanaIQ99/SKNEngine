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

	/// <summary>
	/// コンストラクタ
	/// </summary>
	OBJ3D();

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// OBJ生成
	/// </summary>
	/// <returns></returns>
	static OBJ3D* Create();

	/// <summary>
	/// ライト設定
	/// </summary>
	/// <param name="light"></param>
	static void SetLight(LightGroup* light) { OBJ3D::lightGroup = light; }

private:

	// コマンドリスト

	static LightGroup* lightGroup;


public:

	/// <summary>
	/// モデルセット
	/// </summary>
	/// <param name="_model"></param>
	void SetModel(ObjModel* _model) { model=_model; }

	/// <summary>
	/// 初期化(生成用)
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera"></param>
	void Update(ViewProjection* camera);

	/// <summary>
	/// 描画
	/// </summary>
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

	ObjModel* model;



	static PipelineSet ObjPipeline;
	 


};
