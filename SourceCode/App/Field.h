#pragma once
//フィールド
#include"ObjModel.h"
#include"OBJ3D.h"
#include"Camera.h"

class Field 
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_camera"></param>
	void Init(Camera* _camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エリア制限範囲取得
	/// </summary>
	/// <returns></returns>
	static float GetArea();

	/// <summary>
	/// 上昇限界値
	/// </summary>
	/// <returns></returns>
	float GetUpArea();

private:

	Camera* camera;

	std::unique_ptr<OBJ3D> ground;
	std::unique_ptr<ObjModel> ground_model;

	std::unique_ptr<OBJ3D> skydome;
	std::unique_ptr<ObjModel> skydome_model;

	const float UpAreaLimit = 100.0f;

	static const float AreaLimit;

};

