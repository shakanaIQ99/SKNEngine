#pragma once
#include"HomingBullet.h"
#include"NormalBullet.h"
#include"ObjModel.h"
#include"Bullet.h"

class BulletManager
{
public:


	BulletManager();

	static BulletManager* GetInstance();
	
	static void CreateHomingBullet(ObjModel* model, Vector3 position, Vector3 velocity, Vector3* Target, float size, float BulletSpeed, const Tag& _tag);
	static void CreateNormalBullet(ObjModel* model,Vector3 position, Vector3 velocity, float size, float BulletSpeed, const Tag& _tag);

	
	static void ManageBulletUpdate();
	static void clear();
	
	static const std::list<std::unique_ptr<Bullet>>& GetBulletList() { return GetInstance()->manageBulletList; };

private:
	void Initialize();
	static std::list<std::unique_ptr<Bullet>> manageBulletList;


};

