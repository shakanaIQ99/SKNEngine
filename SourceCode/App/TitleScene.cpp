#include "TitleScene.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "SceneControl.h"

void TitleScene::Initialize()
{
	preTitle = std::make_unique<Sprite2D>();
	preTitle->Initialize("Title");
	preTitle->Wt.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 4.5f ,0.0f };

	preTitle2 = std::make_unique<Sprite2D>();
	preTitle2->Initialize("Title2");
	preTitle2->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f) + 60.0f ,0.0f };
}

void TitleScene::Update()
{
	if (tenmetu > 254.0f || tenmetu < 0.0f)
	{
		decri *= -1.0f;
	}
	tenmetu += decri;
	preTitle2->Wt.color = { tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f ,tenmetu / 255.0f };

	if (Input::GetPadButtonDown(XINPUT_GAMEPAD_A) || Input::GetPressKey(DIK_END))
	{
		SceneControl::GetInstance()->Change("GAME");
	}

	preTitle->Update();
	preTitle2->Update();
}

void TitleScene::Draw()
{
	SpriteCommon::PreDraw();
	preTitle->Draw();
	preTitle2->Draw();


}

void TitleScene::Finalize()
{
}
