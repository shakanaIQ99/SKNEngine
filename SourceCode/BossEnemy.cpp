#include "BossEnemy.h"

void BossEnemy::Init()
{
	ModelInit("chr_sword");
	transform.scale_ = { 4.0f,4.0f,4.0f };
	St->color = { 1.0f,0,0,1.0f };
}

void BossEnemy::Update()
{
	switch (bossmove)
	{
	case NONE:
		break;
	case SIMPLESHOT:
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
}

void BossEnemy::DrawUI()
{
}

void BossEnemy::MoveTabele()
{
}

void BossEnemy::SimpleShot()
{
}
