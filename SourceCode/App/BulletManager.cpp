#include "BulletManager.h"

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::Initlize(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity)
{
	St->Wt.translation_ = position;
	St->Wt.rotation_ = rota;
	St->Wt.scale_ = { 0.5f,0.5f,0.5f };

	Velocity_ = velocity;
}

void BulletManager::Update()
{
}

void BulletManager::Draw()
{
}

void BulletManager::OnCollision()
{
	isDead_ = true;
}

XMFLOAT3 BulletManager::GetWorldPosition()
{
	return St->Wt.translation_;
}

XMFLOAT3 BulletManager::GetScale()
{
	return St->Wt.scale_;
}
