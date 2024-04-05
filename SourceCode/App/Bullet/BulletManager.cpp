#include "BulletManager.h"
std::list<std::unique_ptr<Bullet>> BulletManager::manageBulletList;

BulletManager::BulletManager()
{
}

BulletManager* BulletManager::GetInstance()
{
	static BulletManager instance;
	return &instance;
}

void BulletManager::CreateHomingBullet(ObjModel* Model, Vector3 Position, Vector3 Velocity, Vector3* Target, float Size, float BulletSpeed, const Tag& Tag)
{
	GetInstance()->manageBulletList.emplace_back(std::make_unique<HomingBullet>(Model, Position, Velocity, Target, Size, BulletSpeed, Tag));
}

void BulletManager::CreateNormalBullet(ObjModel* Model, Vector3 Position, Vector3 Velocity, float Size, float BulletSpeed, const Tag& Tag)
{
	GetInstance()->manageBulletList.emplace_back(std::make_unique<NormalBullet>(Model, Position, Velocity, Size, BulletSpeed, Tag));
}

void BulletManager::ManageBulletUpdate()
{
	
	for (std::list<std::unique_ptr<Bullet>>::iterator itr = GetInstance()->manageBulletList.begin(); itr != GetInstance()->manageBulletList.end();) {
		Bullet* obj = (*itr).get();
		if (obj->isdead()) {
			itr = GetInstance()->manageBulletList.erase(itr);
			continue;
		}

		obj->Update();
		obj->Draw();

		itr++;
	}
	
}

void BulletManager::Clear()
{
	GetInstance()->manageBulletList.clear();
}

void BulletManager::Initialize()
{
	GetInstance()->manageBulletList.clear();
}

