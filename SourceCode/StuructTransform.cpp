#include "StuructTransform.h"

Camera* StuructTransform::camera = nullptr;
SpriteCommon* StuructTransform::spCommon = nullptr;
TextureManager* StuructTransform::texMana = nullptr;

void StuructTransform::SetStruct(Camera* _camera, SpriteCommon* spcommon, TextureManager* texmana)
{
	camera = _camera;
	spCommon = spcommon;
	texMana = texmana;
}

void StuructTransform::ModelInit(const string& modelname, bool smooth)
{
	model.reset(ObjModel::LoadFromOBJ(modelname, smooth));

	St.reset(OBJ3D::Create(&transform));
	St->SetModel(model.get());

}
