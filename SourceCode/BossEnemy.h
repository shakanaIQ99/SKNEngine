#pragma once
#include"StuructTransform.h"
//#include"Player.h"

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
	

private:
	WorldTransform* player;
	
	Pattern bossmove;

};

