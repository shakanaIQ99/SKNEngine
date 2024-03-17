#include "HomingBullet.h"

HomingBullet::HomingBullet(ObjModel* model, const Vector3& position, const Vector3& Target, float size, float BulletSpeed, const Tag& _tag)
{
	ModelInit(model);
	St->Wt.translation_ = position;
	St->Wt.scale_ = { size,size,size };
	TargetPos = Target;
	bulletspeed = BulletSpeed;
	tag = _tag;
}

void HomingBullet::Update()
{
	Vector3 toTarget = TargetPos - St->Wt.translation_;

	Velocity_ = slerp(Velocity_, toTarget, 0.5f) * 2.0f;

	St->Wt.translation_ = St->Wt.translation_ + Velocity_;

	St->Update(camera->getView());

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void HomingBullet::Draw()
{
	St->Draw();
}

