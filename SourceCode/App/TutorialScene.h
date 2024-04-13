#pragma once
#include "BaseScene.h"
class TutorialScene:public BaseScene
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void Finalize() override;
};

