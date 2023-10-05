#pragma once
#include"OBJ3D.h"
#include"ObjModel.h"
#include"Camera.h"
#include"Sprite2D.h"
#include"SpriteCommon.h"
class StuructTransform
{
public:

	//各シングルトーン化したら不要になるかも
	static void SetStruct(Camera* _camera, SpriteCommon* spcommon, TextureManager* texmana);


	/// <summary>
	/// 新しくモデルをロード
	/// </summary>
	/// <param name="modelname"></param>
	/// <param name="smooth"></param>
	void ModelInit(const string& modelname,bool smooth=false);

	/// <summary>
	/// モデルそのものをロード
	/// </summary>
	/// <param name="_model"></param>
	void ModelInit(ObjModel* _model);

	
	std::unique_ptr<OBJ3D> St;
	std::unique_ptr<ObjModel> model;
protected:

	static SpriteCommon* spCommon;
	static TextureManager* texMana;
	static Camera* camera;

};

