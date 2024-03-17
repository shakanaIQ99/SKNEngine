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

	//デスフラグ
	bool isDead_ = false;

	//速度
	Vector3 Velocity_;

	Tag tag = Tag::NONE;

};

