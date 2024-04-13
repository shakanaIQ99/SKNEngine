#pragma once
#include "BaseScene.h"
class ResultScene:public BaseScene
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void Finalize() override;
};

