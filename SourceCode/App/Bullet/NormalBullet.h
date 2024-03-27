#pragma once
#include "Bullet.h"

class NormalBullet :public Bullet
{
public:
	NormalBullet(ObjModel* model, Vector3 position, Vector3 velocity, float size, float BulletSpeed, const Tag& _tag);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	void Draw() override;

	Vector3 GetVec() { return Velocity_; }
	
	
private:

	

	
	//弾速
	float bulletspeed = 0;

	//寿命<frm>
	static const int32_t LifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = LifeTime;

	
};
