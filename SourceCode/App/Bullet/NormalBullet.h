#pragma once
#include "Bullet.h"

class NormalBullet :public Bullet
{
public:
	NormalBullet(ObjModel* Model, Vector3 Position, Vector3 Velocity, float Size, float BulletSpeed, const Tag& Tag);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	void Draw() override;

	Vector3 GetVec() { return velocity; }
	
	
private:

	

	
	//弾速
	float bulletSpeed = 0;

	//寿命<frm>
	static const int32_t lifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer = lifeTime;

	
};
