#pragma once
#include "BaseScene.h"
#include "Sprite2D.h"
#include "Camera.h"
#include "LightGroup.h"
#include "OBJ3D.h"
#include "Field.h"

enum class TitleState
{
	
};

class TitleScene:public BaseScene
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void Finalize() override;

private:
	Camera camera;
	LightGroup* light = nullptr;

	std::unique_ptr<Sprite2D> preTitle;

	std::unique_ptr<Sprite2D> preTitle2;

	float tenmetu = 255.0f;

	float decri = 3.0f;

};

