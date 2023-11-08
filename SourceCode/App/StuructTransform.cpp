#include "StuructTransform.h"

Camera* StuructTransform::camera = nullptr;
SpriteCommon* StuructTransform::spCommon = nullptr;

void StuructTransform::SetStruct(Camera* _camera, SpriteCommon* spcommon)
{
	camera = _camera;
	spCommon = spcommon;
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
	St->Initialize();
	St->SetModel(_model);
}
