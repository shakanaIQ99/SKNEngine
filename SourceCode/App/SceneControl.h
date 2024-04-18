#pragma once
#include"SceneChanger.h"
#include"SceneManager.h"
#include"Sprite2D.h"
#include<memory>
#include"ImguiManager.h"

class SceneControl
{

private:
	
	std::string nextScene{};
public:
	void Initialize();
	void Update();
	void Draw();
public:
	void Change(const std::string NextScene);

	void SetWin(bool IsWin);
	bool IsWin() {
		return isWin;
	}

	void ImGuiMode();

public:
	static SceneControl* GetInstance();
private:

	std::unique_ptr<Sprite2D> sceneChangeTexture;
	std::unique_ptr<Sprite2D> loading;

	bool loadFlag = false;


	int sceneChangeTimer = 0;

	const int sceneChangeTime = 30;

	float sceneAlpha = 255.0f;

	bool isActive = false;

	bool isWin = false;

	SceneControl() = default;
	~SceneControl() = default;
	SceneControl(const SceneControl&) = delete;
	const SceneControl& operator=(const SceneControl&) = delete;
};
