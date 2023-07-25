#pragma once
#include"StuructTransform.h"
#include"Player.h"

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

	void SetPlayer(Player* pl)
	{
		player = pl;
	}

	void Init();

	void Update();

	void Draw();

	void DrawUI();

	void MoveTabele();
	

private:
	Player* player;
	
	Pattern bossmove;

};

