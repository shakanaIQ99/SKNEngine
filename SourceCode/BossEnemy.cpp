#include "BossEnemy.h"

void BossEnemy::Init()
{
	ModelInit("chr_sword");
	transform.scale_ = { 1.0f,1.0f,1.0f };
}

void BossEnemy::Update()
{
	St->Update(camera->getView());
}

void BossEnemy::Draw()
{
	St->Draw();
}

void BossEnemy::DrawUI()
{
}
