#pragma once
#include"StuructTransform.h"

class BossEnemy : public StuructTransform
{
public:

	enum Pattern
	{
		NONE,
		SIMPLESHOT,
		CHARGE,
		LASER
	};

	void SetPlayer(WorldTransform* pl)
	{
		player = pl;
	}

	void Init();

	void Update();

	void Draw();

	void DrawUI();

	void MoveTabele();

	void SimpleShot();
	

private:
	WorldTransform* player;
	
	Pattern bossmove;

};

