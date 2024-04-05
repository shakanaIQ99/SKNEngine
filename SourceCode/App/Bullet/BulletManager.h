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
	
	static void CreateHomingBullet(ObjModel* Model, Vector3 Position, Vector3 Velocity, Vector3* Target, float Size, float BulletSpeed, const Tag& Tag);
	static void CreateNormalBullet(ObjModel* Model,Vector3 Position, Vector3 Velocity, float Size, float BulletSpeed, const Tag& Tag);

	
	static void ManageBulletUpdate();
	static void Clear();
	
	static const std::list<std::unique_ptr<Bullet>>& GetBulletList() { return GetInstance()->manageBulletList; };

private:
	void Initialize();
	static std::list<std::unique_ptr<Bullet>> manageBulletList;


};

