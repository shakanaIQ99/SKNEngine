#pragma once
#include"Bullet.h"
#include "DeathParticle.h"
class HomingBullet :public Bullet
{
public:

	HomingBullet(ObjModel* Model, Vector3 Position, Vector3 Velocity,Vector3* Target, float Size, float BulletSpeed, const Tag& Tag);
	
	void Update() override;

	void Draw() override;


private:

	//速度
	Vector3* targetPos;

	Vector3 firstPos;
	//弾速
	float bulletSpeed = 0;

	float homingPower = 0;

	//演出周り

	std::list<std::unique_ptr<DeathParticle>> deathPaticles;


	//寿命<frm>
	static const int32_t lifeTime = 60 * 10;

	//デスタイマー
	int32_t deathTimer = lifeTime;

	int trailCount = 0;


};

