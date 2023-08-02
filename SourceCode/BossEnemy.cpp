#include "BossEnemy.h"
#include"Player.h"
void BossEnemy::Init()
{
	ModelInit("chr_sword");
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


	St->Update(camera->getView());
}

void BossEnemy::Draw()
{
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
	AimMode = true;
	bossmove = SIMPLESHOT;

}

void BossEnemy::SimpleShot()
{
	if (TargetTimer > 0.0f)
	{
		if(TargetTimer > 10.0f){ TargetPos = player->translation_; }
		TargetTimer--;
	}
	else
	{


		AimMode = false;
		XMFLOAT3 BulletVec = TargetPos - transform.translation_;
		normalize(BulletVec);

		bossmove = NONE;
	}





}
