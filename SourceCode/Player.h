#pragma once
#include<ObjModel.h>
#include<OBJ3D.h>
#include<WorldTronsform.h>
#include<Camera.h>
#include<Input.h>



class Player
{
public:
	void SetStruct(ObjModel* _model,OBJ3D* _obj,Camera* _camera,Input* _input);

	void Init();

	void Update();

	void Draw();

	XMFLOAT3 GetPos()
	{
		return player->Wt->translation_;
	}

private:
	Input* input;

	

	

	Camera* camera;

	OBJ3D* player = nullptr;
	ObjModel* player_model = nullptr;
};

