#pragma once
#include"ObjModel.h"
#include"OBJ3D.h"
#include"Camera.h"

class Field 
{
public:

	void Init(Camera* _camera);

	void Update();

	void Draw();

	static float GetArea();

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

