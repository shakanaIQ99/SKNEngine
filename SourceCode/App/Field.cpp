#include "Field.h"

const float Field::AreaLimit = 500.0f;

void Field::Init(Camera* _camera)
{
	ground_model.reset(ObjModel::LoadFromOBJ("555"));
	ground.reset(OBJ3D::Create());
	ground->SetModel(ground_model.get());
	ground->Wt.scale_ = { AreaLimit,0.0f,AreaLimit };

	skydome_model.reset(ObjModel::LoadFromOBJ("skydome"));
	skydome.reset(OBJ3D::Create());
	skydome->SetModel(skydome_model.get());

	camera = _camera;
}

void Field::Update()
{
	ground->Update(camera->getView());
	skydome->Update(camera->getView());

}

void Field::Draw()
{
	ground->Draw();
	skydome->Draw();

}

float Field::GetArea()
{
	return AreaLimit;
}

float Field::GetUpArea()
{
	return UpAreaLimit;
}
