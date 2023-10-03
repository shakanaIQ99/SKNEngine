#pragma once
#include"ObjModel.h"
#include"OBJ3D.h"

class Field 
{
public:

	void Init();

	void Draw();

	void GetArea();

private:

	std::unique_ptr<OBJ3D> St;
	std::unique_ptr<ObjModel> model;

};

