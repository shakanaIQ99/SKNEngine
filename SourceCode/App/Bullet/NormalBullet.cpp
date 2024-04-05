#include "NormalBullet.h"

NormalBullet::NormalBullet(ObjModel* model, Vector3 position, Vector3 velocity, float size, float BulletSpeed, const Tag& _tag)
{
	ModelInit(model);
	St->Wt.translation_ = position;
	St->Wt.scale_ = { size,size,size };
	Velocity_ = velocity;
	bulletspeed = BulletSpeed;
	tag = _tag;
}

void NormalBullet::Update()
{
	St->Wt.translation_ += Velocity_ * bulletspeed;

	St->Update(camera->GetView());

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer_ <= 0)
	{
		onDead();
	}
}

void NormalBullet::Draw()
{
	St->Draw();
}

