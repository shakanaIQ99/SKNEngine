#include "BossEnemy.h"

void BossEnemy::Init()
{
	ModelInit("chr_sword");
	transform.scale_ = { 1.0f,1.0f,1.0f };
}

void BossEnemy::Update()
{
	switch (bossmove)
	{
	case BossEnemy::NONE:
		break;
	case BossEnemy::SIMPLESHOT:
		break;
	case BossEnemy::CHARGE:
		break;
	case BossEnemy::LASER:
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
