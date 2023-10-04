#include "Field.h"

void Field::Init(Camera* _camera)
{
	model.reset(ObjModel::LoadFromOBJ("555"));
	St.reset(OBJ3D::Create());
	St->SetModel(model.get());
	St->Wt.scale_ = { AreaLimit,0.0f,AreaLimit };

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
