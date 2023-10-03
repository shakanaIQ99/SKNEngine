#include "Field.h"

void Field::Init()
{
	model.reset(ObjModel::LoadFromOBJ("ground"));
	St.reset(OBJ3D::Create());
	St->SetModel(model.get());
}

void Field::Draw()
{
}

void Field::GetArea()
{
}
