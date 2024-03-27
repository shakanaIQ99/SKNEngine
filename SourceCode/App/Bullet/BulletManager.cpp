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

void BulletManager::CreateHomingBullet(ObjModel* model, Vector3 position, Vector3 velocity, Vector3* Target, float size, float BulletSpeed, const Tag& _tag)
{
	GetInstance()->manageBulletList.emplace_back(std::make_unique<HomingBullet>(model, position,velocity, Target, size, BulletSpeed, _tag));
}

void BulletManager::CreateNormalBullet(ObjModel* model, Vector3 position, Vector3 velocity, float size, float BulletSpeed, const Tag& _tag)
{
	GetInstance()->manageBulletList.emplace_back(std::make_unique<NormalBullet>(model, position, velocity, size, BulletSpeed, _tag));
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

void BulletManager::clear()
{
	GetInstance()->manageBulletList.clear();
}

void BulletManager::Initialize()
{
	GetInstance()->manageBulletList.clear();
}

