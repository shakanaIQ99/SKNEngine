#include "HomingBullet.h"

HomingBullet::HomingBullet(ObjModel* model, const Vector3& position, const Vector3& velocity, Vector3* Target, float size, float BulletSpeed, const Tag& _tag)
{
	ModelInit(model);
	St->Wt.translation_ = position;
	firstPos = position;
	St->Wt.scale_ = { size,size,size };
	Velocity_ = velocity;
	TargetPos = Target;
	bulletspeed = BulletSpeed;
	tag = _tag;
	deathTimer_ = LifeTime;
	St->color = { 1.0f,0,0,1.0f };

	HomingPower = 0.4f;
}

void HomingBullet::Update()
{
	Vector3 toTarget(TargetPos->x - St->Wt.translation_.x, TargetPos->y - St->Wt.translation_.y, TargetPos->z - St->Wt.translation_.z);

	Velocity_ = slerp(Velocity_, toTarget, HomingPower) * 2.0f;

	St->Wt.translation_ = St->Wt.translation_ + Velocity_;


	Vector3 a = Vector3(TargetPos->x - firstPos.x, TargetPos->y - firstPos.y, TargetPos->z - firstPos.z);

	Vector3 b = St->Wt.translation_ - firstPos;

	float lenA = a.length();

	float lenB = b.length();

	if (lenA < lenB)
	{
		HomingPower = 0;
	}

	Vector3 Flont = { 0,0,1.0f };
	b.normalize();
	a.normalize();


	float p_pos = atan2(a.x, a.z);
	float c_vec = atan2(b.x, b.z);

	St->Wt.rotation_.y = (p_pos + c_vec);

	St->Update(camera->getView());
	if (--deathTimer_ <= 0 )
	{
		onDead();
	}

}

void HomingBullet::Draw()
{
	St->Draw();
}

