#include "SceneControl.h"
#include "Easing.h"



void SceneControl::Initialize()
{
	sceneChangeTexture = std::make_unique<Sprite2D>();
	sceneChangeTexture->Initialize("Scene");
	sceneChangeTexture->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 2.0f) ,0.0f };
	isActive = false;
	loading = std::make_unique<Sprite2D>();
	loading->Initialize("Load");
	loadFlag = false;
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
				loadFlag = true;
			}
		}
		else
		{
			loadFlag = false;
			sceneChangeTimer--;
			if (sceneChangeTimer <= 0)
			{
				sceneChangeTimer = 0;
				isActive = false;
			}
		}
	}


#ifdef _DEBUG
	ImGuiMode();
#endif

	sceneAlpha = EaseOutSine(0, 255.0f, static_cast<float>(sceneChangeTimer), static_cast<float>(sceneChangeTime));
	sceneChangeTexture->Wt.color = { sceneAlpha / 255.0f ,sceneAlpha / 255.0f ,sceneAlpha / 255.0f ,sceneAlpha / 255.0f };
	sceneChangeTexture->Update();
}

void SceneControl::Draw()
{

	sceneChangeTexture->Draw();
	if (loadFlag)loading->Draw(static_cast<float>(DxWindow::window_width - 480), static_cast<float>(DxWindow::window_height - 80), static_cast<float>(DxWindow::window_width), static_cast<float>(DxWindow::window_height));

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

void SceneControl::ImGuiMode()
{
	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x, ImGui::GetMainViewport()->WorkPos.y +700 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Scene", NULL, window_flags);

	static int SceneNum = 0;
	const char* MoveModes[] = { "Title", "Game", "Result"};
	ImGui::Combo("##MovemodeNumCombo", &SceneNum, MoveModes, IM_ARRAYSIZE(MoveModes));
	ImGui::SameLine();
	if (ImGui::Button("Change"))
	{
		switch (SceneNum)
		{
		case 0:
			Change("TITLE");
			break;
		case 1:
			Change("GAME");
			break;
		case 2:
			Change("RESULT");
			break;
		}

	}
	ImGui::Text("nowScene::%s", MoveModes[static_cast<int>(SceneNum)]);


	ImGui::End();
}

SceneControl* SceneControl::GetInstance()
{
	static SceneControl instance;
	return &instance;
}
