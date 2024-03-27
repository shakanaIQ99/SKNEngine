#pragma once
#include"Bullet.h"
class HomingBullet :public Bullet
{
public:

	HomingBullet(ObjModel* model, Vector3 position, Vector3 velocity,Vector3* Target, float size, float BulletSpeed, const Tag& _tag);
	
	void Update() override;

	void Draw() override;


private:

	//速度
	Vector3* TargetPos;

	Vector3 firstPos;
	//弾速
	float bulletspeed = 0;

	float HomingPower = 0;

	//寿命<frm>
	static const int32_t LifeTime = 60 * 10;

	//デスタイマー
	int32_t deathTimer_ = LifeTime;


};

