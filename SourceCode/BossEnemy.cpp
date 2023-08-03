#include "BossEnemy.h"
#include"Player.h"
void BossEnemy::Init()
{
	ModelInit("chr_sword");
	EnemyNormalBullet::SetModel(ObjModel::LoadFromOBJ("maru"));
	transform.scale_ = { 4.0f,4.0f,4.0f };
	St->color = { 1.0f,0,0,1.0f };
	LeserPoint.Init();
}

void BossEnemy::Update()
{
	switch (bossmove)
	{
	case NONE:
		MoveTabele();

		break;
	case SIMPLESHOT:

		SimpleShot();

		break;
	case CHARGE:
		break;
	case LASER:
		break;
	}

	for (std::unique_ptr<EnemyNormalBullet>& bullet : Normalbullets_)
	{
		bullet->Update();
	}

	St->Update(camera->getView());
}

void BossEnemy::Draw()
{
	for (std::unique_ptr<EnemyNormalBullet>& bullet : Normalbullets_)
	{
		bullet->Draw();
	}

	St->Draw();
	XMFLOAT3 Head = transform.translation_;

	Head.y += transform.scale_.y;
	if (AimMode)
	{
		LeserPoint.Draw(Head,TargetPos);
	}
}

void BossEnemy::DrawUI()
{
}

void BossEnemy::MoveTabele()
{
	TargetTimer = TargetTime;
	bossmove = SIMPLESHOT;
	BurstTime= BurstNum * BurstRate;

}

void BossEnemy::SimpleShot()
{
	if (TargetTimer > 0)
	{
		if (TargetTimer > 10)
		{
			TargetPos = player->translation_;

		}
		TargetTimer--;
		AimMode = true;
	}
	else
	{
		XMFLOAT3 Head = transform.translation_;

		Head.y += transform.scale_.y;
		AimMode = false;
		XMFLOAT3 BulletVec = TargetPos - Head;
		normalize(BulletVec);

		BulletVec *= 2.0f;

		if (BurstTime % BurstRate == 0)
		{
			std::unique_ptr <EnemyNormalBullet> newBullet = std::make_unique<EnemyNormalBullet>();
			newBullet->Initlize(Head, transform.rotation_, BulletVec);

			Normalbullets_.push_back(std::move(newBullet));

		}
		BurstTime--;
		if (BurstTime <= 0)
		{
			bossmove = NONE;

		}
	}


	



}

void BossEnemy::Bulletremove()
{
	Normalbullets_.remove_if([](std::unique_ptr<EnemyNormalBullet>& bullet)
		{
			return bullet->IsDead();
		});

}
