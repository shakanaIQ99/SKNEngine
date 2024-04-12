#include "StuructTransform.h"

Camera* StuructTransform::camera = nullptr;
TextureManager* StuructTransform::texMana = nullptr;

void StuructTransform::SetStruct(Camera* _camera,TextureManager* texmana)
{
	camera = _camera;
	texMana = texmana;
}

void StuructTransform::ModelInit(const string& modelname, bool smooth)
{
	model.reset(ObjModel::LoadFromOBJ(modelname, smooth));

	St.reset(OBJ3D::Create());
	St->SetModel(model.get());

}

void StuructTransform::ModelInit(ObjModel* _model)
{
	St.reset(OBJ3D::Create());
	St->SetModel(_model);
}
