#include "SceneControl.h"
#include "Easing.h"



void SceneControl::Initialize()
{
	sceneChangeTexture = std::make_unique<Sprite2D>();
	sceneChangeTexture->Initialize("Scene");
	sceneChangeTexture->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f) ,0.0f };
	isActive = false;
}

void SceneControl::Update()
{
	if (isActive)
	{
		if (SceneManager::GetInstance()->GetSceneName() != nextScene)
		{
			sceneChangeTimer++;
			if (sceneChangeTimer >= sceneChangeTime)
			{
				SceneManager::GetInstance()->ChangeScene(nextScene);
			}
		}
		else
		{
			sceneChangeTimer--;
			if (sceneChangeTimer <= 0)
			{
				sceneChangeTimer = 0;
				isActive = false;
			}
		}
	}


	sceneAlpha = EaseOutSine(0, 255.0f, static_cast<float>(sceneChangeTimer), static_cast<float>(sceneChangeTime));
	sceneChangeTexture->Wt.color = { sceneAlpha / 255.0f ,sceneAlpha / 255.0f ,sceneAlpha / 255.0f ,sceneAlpha / 255.0f };
	sceneChangeTexture->Update();
}

void SceneControl::Draw()
{

	sceneChangeTexture->Draw();

}

void SceneControl::Change(const std::string NextScene)
{
	nextScene = NextScene;
	isActive = true;
}

void SceneControl::SetWin(bool IsWin)
{
	isWin = IsWin;
}

SceneControl* SceneControl::GetInstance()
{
	static SceneControl instance;
	return &instance;
}
