#pragma once
#include "BaseScene.h"
#include "Sprite2D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "OBJ3D.h"
#include "Field.h"
#include "ObjModel.h"
#include "Field.h"
#include "Player.h"

class TutorialScene:public BaseScene
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void Finalize() override;

private:

	void ImGuiMode();

	Camera camera;

	Player player;

	LightGroup* light = nullptr;


	std::unique_ptr<OBJ3D> skydome;
	std::unique_ptr<ObjModel> skydome_model;

	std::unique_ptr<Sprite2D> SceneItem[2];

};

