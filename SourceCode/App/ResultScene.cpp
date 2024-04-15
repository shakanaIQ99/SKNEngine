#include "ResultScene.h"
#include"Input.h"
#include"SpriteCommon.h"
#include"SceneControl.h"

void ResultScene::Initialize()
{
	clearSc = std::make_unique<Sprite2D>();
	clearSc->Initialize("Clear");
	clearSc->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f) ,0.0f };

	if (!SceneControl::GetInstance()->IsWin())
	{
		clearSc->SetTexture("GameOver");
	}
}

void ResultScene::Update()
{
	if (Input::GetPadButtonDown(XINPUT_GAMEPAD_A) || Input::GetPressKey(DIK_END))
	{
		SceneControl::GetInstance()->Change("TITLE");
	}

	clearSc->Update();
}

void ResultScene::Draw()
{
	SpriteCommon::PreDraw();
	clearSc->Draw();
}

void ResultScene::Finalize()
{
}
