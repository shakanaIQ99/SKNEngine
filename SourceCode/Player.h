#pragma once
#include<ObjModel.h>
#include<OBJ3D.h>
#include<WorldTronsform.h>
#include<ViewProjection.h>
#include<Input.h>



class Player
{
public:
	void SetStruct(ObjModel* _model,OBJ3D* _obj,ViewProjection* _camera,Input* _input);

	void Init();

	void Update();

	void Draw();

	XMFLOAT3 GetPos()
	{
		return player->Wt->translation_;
	}

private:
	Input* input;

	

	

	ViewProjection* camera;

	OBJ3D* player = nullptr;
	ObjModel* player_model = nullptr;
};

