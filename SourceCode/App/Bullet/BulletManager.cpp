#include "BulletManager.h"

BulletManager::BulletManager()
{
}

BulletManager* BulletManager::GetInstance()
{
	static BulletManager instance;
	return &instance;
}

void BulletManager::CreateHomingBullet(ObjModel* model, const Vector3& position, const Vector3& Target, float size, float BulletSpeed, const Tag& _tag)
{
	GetInstance()->manageBulletList.emplace_back(std::make_unique<HomingBullet>(model, position, Target, size, BulletSpeed, _tag));
}

void BulletManager::CreateNormalBullet(ObjModel* model, const Vector3& position, const Vector3& velocity, float size, float BulletSpeed, const Tag& _tag)
{
	GetInstance()->manageBulletList.emplace_back(std::make_unique<NormalBullet>(model, position, velocity, size, BulletSpeed, _tag));
}

void BulletManager::ManageBulletUpdate()
{
	if (!GetInstance()->manageBulletList.empty())
	{
		for (std::list<std::unique_ptr<Bullet>>::iterator itr = GetInstance()->manageBulletList.begin(); itr != GetInstance()->manageBulletList.end();) {
			Bullet* obj = (*itr).get();
			if (obj->isDead_) {
				itr = GetInstance()->manageBulletList.erase(itr);
				continue;
			}

			obj->Update();
			obj->Draw();

			itr++;
		}
	}
}

void BulletManager::clear()
{
	GetInstance()->manageBulletList.clear();
}

void BulletManager::Initialize()
{
	GetInstance()->manageBulletList.clear();
}

