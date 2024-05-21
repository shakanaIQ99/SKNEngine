#include "TitleScene.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "SceneControl.h"
#include "ImGuiManager.h"
#include "Easing.h"



void TitleScene::Initialize()
{
	preTitle = std::make_unique<Sprite2D>();
	preTitle->Initialize("Title");
	preTitle->Wt.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 4.5f ,0.0f };

	preTitle2 = std::make_unique<Sprite2D>();
	preTitle2->Initialize("Title2");
	preTitle2->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f) + 100.0f ,0.0f };

	SceneItem[0] = std::make_unique<Sprite2D>();
	SceneItem[0]->Initialize("Scene");
	SceneItem[1] = std::make_unique<Sprite2D>();
	SceneItem[1]->Initialize("Scene");

	light = LightGroup::Create();
	OBJ3D::SetLight(light);
	camera.Initialize();

	skydome_model.reset(ObjModel::LoadFromOBJ("skydome", true));
	skydome.reset(OBJ3D::Create());
	skydome->SetModel(skydome_model.get());

	ship_model.reset(ObjModel::LoadFromOBJ("Ship"));
	ship.reset(OBJ3D::Create());
	ship->SetModel(ship_model.get());

	titlePlayer_model.reset(ObjModel::LoadFromOBJ("Player"));
	titlePlayer.reset(OBJ3D::Create());
	titlePlayer->SetModel(titlePlayer_model.get());

	camera.Reset();

	camera.SetPos(Vector3(30.0f, 15.0f, 100.0f));
	camera.SetRotate(Vector3(0, 3.5f, 0));
	camera.SetTarget(Vector3(0, 0, 0));
	titlePlayer->Wt.scale_ = { 4.0f, 4.0f, 4.0f };
	titlePlayer->Wt.translation_.y = titlePlayer->Wt.scale_.y * 2.0f;

	sceneState = TitleState::TITLEBASE;
	sceneYpos = 0;


}

void TitleScene::Update()
{
	switch (sceneState)
	{
	case TitleState::TITLEBASE:
		if (tenmetu > 254.0f || tenmetu < 0.0f)
		{
			decri *= -1.0f;
		}
		tenmetu += decri;

		if (Input::GetPadButtonDown(XINPUT_GAMEPAD_A) || Input::GetPressKey(DIK_END))
		{
			tenmetu = 255.0f;
			sceneState = TitleState::TITLESTART;
			preTarget = camera.GetView()->Gettarget();
			camera.SetTargetFlag(true);
		}

		break;
	case TitleState::TITLESTART:


		sceneCameraTimer++;

		sceneYpos = EaseOutQuint(0, 100.0f, static_cast<float>(sceneCameraTimer) , static_cast<float>(sceneCameraTime));
		tenmetu = EaseOutQuint(255.0f, 0, static_cast<float>(sceneCameraTimer), static_cast<float>(sceneCameraTime));
		preTitle->Wt.color = { tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f };


		if (sceneCameraTimer > sceneCameraTime)
		{
			sceneState = TitleState::TITLEMOVIE;
		}


		break;

	case TitleState::TITLEMOVIE:

		titlePlayer->Wt.translation_.z = EaseOutSine(0, 1000.0f, static_cast<float>(sceneMoveTimer), static_cast<float>(sceneMoveTime));
		sceneMoveTimer++;
		if (sceneMoveTimer > sceneMoveTime)
		{
			sceneState = TitleState::TITLEEND;
		}

		break;

	case TitleState::TITLEEND:

		SceneControl::GetInstance()->Change("GAME");

		break;
	}

#ifdef _DEBUG
	ImGuiMode();
#endif
	camera.SetTarget(Vector3(EaseOutQuint(preTarget, titlePlayer->Wt.translation_, static_cast<float>(sceneCameraTimer), static_cast<float>(sceneCameraTime))));
	preTitle2->Wt.color = { tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f };
	camera.Update();
	skydome->Update(camera.GetView());
	ship->Update(camera.GetView());
	titlePlayer->Update(camera.GetView());
	preTitle->Update();
	preTitle2->Update();

	SceneItem[0]->Update();
	SceneItem[1]->Update();
}

void TitleScene::Draw()
{

	skydome->Draw();
	ship->Draw();
	titlePlayer->Draw();

	SpriteCommon::PreDraw();
	preTitle->Draw();
	preTitle2->Draw();
	SceneItem[0]->Draw(0, 0, 1280.0f, sceneYpos);
	SceneItem[1]->Draw(0,720.0f - sceneYpos, 1280.0f, 720.0f);


}

void TitleScene::Finalize()
{
	delete light;
}

void TitleScene::ImGuiMode()
{
	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x + 800, ImGui::GetMainViewport()->WorkPos.y + 10 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Player", NULL, window_flags);

	ImGui::Text("Position");
	ImGui::DragFloat("Y", &titlePlayer->Wt.translation_.y, 0.5f);

	ImGui::End();
}


