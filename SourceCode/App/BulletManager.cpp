#include "BulletManager.h"

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::Initlize(const Vector3& position, const Vector3& rota, const Vector3& velocity)
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

Vector3 BulletManager::GetWorldPosition()
{
	return St->Wt.translation_;
}

Vector3 BulletManager::GetScale()
{
	return St->Wt.scale_;
}
