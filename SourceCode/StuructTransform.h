#pragma once
#include<ObjModel.h>
#include<OBJ3D.h>
#include<WorldTronsform.h>
#include<Camera.h>
#include<Sprite2D.h>
#include<SpriteCommon.h>
class StuructTransform
{
public:

	static void SetStruct(Camera* _camera, SpriteCommon* spcommon, TextureManager* texmana);

	void ModelInit(const string& modelname,bool smooth=false);
	void ModelInit(ObjModel* _model);

	
	std::unique_ptr<OBJ3D> St;
	std::unique_ptr<ObjModel> model;
	WorldTransform transform;
protected:

	static SpriteCommon* spCommon;
	static TextureManager* texMana;
	static Camera* camera;

};

