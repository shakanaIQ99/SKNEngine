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

	deathPaticles.remove_if([](std::unique_ptr<DeathParticle>& dp)
		{
			return dp->IsDead();
		});

	

	Vector3 toTarget(targetPos->x - St->Wt.translation_.x, targetPos->y - St->Wt.translation_.y, targetPos->z - St->Wt.translation_.z);

	velocity = Slerp(velocity, toTarget, homingPower) * 2.0f;

	float toTargetY = -toTarget.y;
	Vector3 preToTarget = toTarget;

	preToTarget.y = 0;

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
	velocity.Normalize();
	Flont.Normalize();
	float p_pos = atan2(velocity.x, velocity.z);
	float c_vec = atan2(toTargetY, preToTarget.Length());

	if (++trailCount >= 2)
	{
		std::unique_ptr <DeathParticle> newBullet = std::make_unique<DeathParticle>();
		newBullet->CreateTrailParticle(St->Wt.translation_, St->Wt.rotation_, -velocity, 0.5f, { 0.6f,0.6f,0.6f,1.0f });

		deathPaticles.push_back(std::move(newBullet));
		trailCount = 0;
	}

	

	St->Wt.rotation_.y = (p_pos);
	St->Wt.rotation_.x = (c_vec);
	St->Wt.rotation_.z += 0.1f;
	//パーティクル更新
	for (std::unique_ptr<DeathParticle>& dp : deathPaticles)
	{
		dp->Update();
	}

	St->Update(camera->GetView());
	if (--deathTimer <= 0 )
	{
		onDead();
	}
	if (isdead())
	{
		deathPaticles.clear();
	}

}

void HomingBullet::Draw()
{
	for (std::unique_ptr<DeathParticle>& dp : deathPaticles)
	{
		dp->Draw();
	}
	St->Draw();
}

