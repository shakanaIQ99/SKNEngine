#pragma once
#include "BaseScene.h"
#include"Sprite2D.h"
#include<memory>
class ResultScene:public BaseScene
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void Finalize() override;

private:

	std::unique_ptr<Sprite2D> clearSc;
};

