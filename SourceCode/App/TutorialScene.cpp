#include "TutorialScene.h"
#include"Input.h"
#include"Collision.h"
#include"Easing.h"
#include"DeathParticle.h"
#include"AudioManager.h"
#include"StuructTransform.h"

void TutorialScene::Initialize()
{
	light = LightGroup::Create();
	OBJ3D::SetLight(light);
	camera.Initialize();

	skydome_model.reset(ObjModel::LoadFromOBJ("skydome", true));
	skydome.reset(OBJ3D::Create());
	skydome->SetModel(skydome_model.get());


}

void TutorialScene::Update()
{
	camera.Update();
	skydome->Update(camera.GetView());
}

void TutorialScene::Draw()
{
	skydome->Draw();

	SpriteCommon::PreDraw();

}

void TutorialScene::Finalize()
{
}

void TutorialScene::ImGuiMode()
{
}
