#include "Field.h"

void Field::Init(Camera* _camera)
{
	model.reset(ObjModel::LoadFromOBJ("ground"));
	St.reset(OBJ3D::Create());
	St->SetModel(model.get());

	camera = _camera;
}

void Field::Update()
{
	St->Update(camera->getView());


}

void Field::Draw()
{
	St->Draw();

}

float Field::GetArea()
{
	return AreaLimit;
}

float Field::GetUpArea()
{
	return UpAreaLimit;
}
