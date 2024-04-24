#include "TutorialScene.h"
#include "Input.h"
#include "Collision.h"
#include "Easing.h"
#include "DeathParticle.h"
#include "AudioManager.h"
#include "StuructTransform.h"
#include "BulletManager.h"

void TutorialScene::Initialize()
{
	light = LightGroup::Create();
	OBJ3D::SetLight(light);
	camera.Initialize();

	StuructTransform::SetStruct(&camera);
	skydome_model.reset(ObjModel::LoadFromOBJ("skydome", true));
	skydome.reset(OBJ3D::Create());
	
	skydome->SetModel(skydome_model.get());

	player.Initialize();
	player.Reset();

	BulletManager::Clear();
	camera.SetTarget(&player.prePlayer);

}

void TutorialScene::Update()
{

	//ALLCol();

	light->Update();

	camera.Update();
	skydome->Update(camera.GetView());
	player.Update();
}

void TutorialScene::Draw()
{
	skydome->Draw();
	player.Draw();
	SpriteCommon::PreDraw();

	player.DrawUI();

}

void TutorialScene::Finalize()
{
	delete light;
}

void TutorialScene::ImGuiMode()
{
}
