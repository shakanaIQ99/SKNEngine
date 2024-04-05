#include "HomingBullet.h"

HomingBullet::HomingBullet(ObjModel* Model, Vector3 Position, Vector3 Velocity, Vector3* Target, float Size, float BulletSpeed, const Tag& Tag)
{
	ModelInit(Model);
	St->Wt.translation_ = Position;
	firstPos = Position;
	St->Wt.scale_ = { Size,Size,Size };
	velocity = Velocity;
	targetPos = Target;
	bulletSpeed = BulletSpeed;
	tag = Tag;
	deathTimer = lifeTime;
	St->color = { 1.0f,0,0,1.0f };

	homingPower = 0.3f;
}

void HomingBullet::Update()
{
	Vector3 toTarget(targetPos->x - St->Wt.translation_.x, targetPos->y - St->Wt.translation_.y, targetPos->z - St->Wt.translation_.z);

	velocity = Slerp(velocity, toTarget, homingPower) * 2.0f;

	St->Wt.translation_ = St->Wt.translation_ + velocity;


	Vector3 a = Vector3(targetPos->x - firstPos.x, targetPos->y - firstPos.y, targetPos->z - firstPos.z);

	Vector3 b = St->Wt.translation_ - firstPos;

	float lenA = a.Length();

	float lenB = b.Length();

	if (lenA < lenB)
	{
		homingPower = 0;
	}

	Vector3 Flont = { 0,0,1.0f };
	b.Normalize();
	a.Normalize();


	float p_pos = atan2(a.x, a.z);
	float c_vec = atan2(b.x, b.z);

	St->Wt.rotation_.y = (p_pos + c_vec);

	St->Update(camera->GetView());
	if (--deathTimer <= 0 )
	{
		onDead();
	}

}

void HomingBullet::Draw()
{
	St->Draw();
}

