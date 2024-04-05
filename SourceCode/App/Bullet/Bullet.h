#pragma once
#include "StuructTransform.h"

enum struct Tag
{
	NONE,
	PLAYER,
	ENEMYNORMAL,
	ENEMYHARD,
};

class Bullet :public StuructTransform
{
public:

	virtual ~Bullet(){}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	Vector3 GetVec() { return velocity.GetNormalize(); }

	const bool isdead() { return isDead; }

	void onDead() { isDead = true; }

	//速度
	Vector3 velocity;

	Tag tag = Tag::NONE;
private:
	//デスフラグ
	bool isDead = false;
};

