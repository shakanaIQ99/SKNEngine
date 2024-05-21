#pragma once
#include "BaseScene.h"
#include "Sprite2D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "OBJ3D.h"
#include "Field.h"
#include "ObjModel.h"

enum class TitleState
{
	TITLEBASE,
	TITLESTART,
	TITLEMOVIE,
	TITLEEND
};

class TitleScene:public BaseScene
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void Finalize() override;



private:

	void ImGuiMode();

	Camera camera;

	LightGroup* light = nullptr;

	std::unique_ptr<OBJ3D> ship;
	std::unique_ptr<ObjModel> ship_model;

	std::unique_ptr<OBJ3D> titlePlayer;
	std::unique_ptr<ObjModel> titlePlayer_model;

	std::unique_ptr<OBJ3D> skydome;
	std::unique_ptr<ObjModel> skydome_model;

	std::unique_ptr<Sprite2D> preTitle;

	std::unique_ptr<Sprite2D> preTitle2;

	std::unique_ptr<Sprite2D> SceneItem[2];

	Vector3 preTarget;

	float sceneYpos;

	//各種ムービータイマー

	int sceneCameraTimer = 0;
	const int sceneCameraTime = 120;

	int sceneMoveTimer = 0;
	const int sceneMoveTime = 180;

	float tenmetu = 255.0f;

	float decri = 4.0f;

	TitleState sceneState = TitleState::TITLEBASE;

};

