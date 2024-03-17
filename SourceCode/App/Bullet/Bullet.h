#pragma once
#include "StuructTransform.h"

enum Tag
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

	Vector3 GetVec() { return Velocity_.getnormalize(); }

	const bool isdead() { return isDead_; }

	void onDead() { isDead_ = true; }

	//速度
	Vector3 Velocity_;

	Tag tag = Tag::NONE;
private:
	//デスフラグ
	bool isDead_ = false;
};

