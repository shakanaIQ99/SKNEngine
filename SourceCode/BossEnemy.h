#pragma once
#include"StuructTransform.h"
#include"EnemyNormalBullet.h"
#include"Draw3DLine.h"

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




	
	const std::list<std::unique_ptr<EnemyNormalBullet>>& GetBullets() { return Normalbullets_; };

private:
	WorldTransform* player;
	
	Pattern bossmove;

	Draw3DLine LeserPoint;

	//’eŠÖ˜A
	std::list<std::unique_ptr<EnemyNormalBullet>> Normalbullets_;

	XMFLOAT3 TargetPos = { 0,0,0 };

	const float TargetTime = 60.0f * 2.0f;

	float TargetTimer = 0.0f;

	bool AimMode = false;

};

