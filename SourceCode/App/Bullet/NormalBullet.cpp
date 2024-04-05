#include "NormalBullet.h"

NormalBullet::NormalBullet(ObjModel* Model, Vector3 Position, Vector3 Velocity, float Size, float BulletSpeed, const Tag& Tag)
{
	ModelInit(Model);
	St->Wt.translation_ = Position;
	St->Wt.scale_ = { Size,Size,Size };
	velocity = Velocity;
	bulletSpeed = BulletSpeed;
	tag = Tag;
}

void NormalBullet::Update()
{
	St->Wt.translation_ += velocity * bulletSpeed;

	St->Update(camera->GetView());

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer <= 0)
	{
		onDead();
	}
}

void NormalBullet::Draw()
{
	St->Draw();
}

