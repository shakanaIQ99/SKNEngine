#pragma once
#include"StuructTransform.h"
#include"EnemyNormalBullet.h"
#include"Draw3DLine.h"
#include"Player.h"

enum struct AtkPattern
{
	NONE,
	SIMPLESHOT,
	CHARGE,
	LASER,
	MISSILE
};

enum struct MovePattern
{
	NONE,
	BACK,
	FANSHAPE,
	FLY
};

class BossEnemy : public StuructTransform
{
public:

	BossEnemy();
	

	void SetPlayer(Player* pl)
	{
		player = pl;
	}

	

	void Init();

	void Reset();

	void Update();

	void Damege(float dmg);

	void Draw();

	void DrawUI();

	AtkPattern GetAtkPattern()
	{
		return BossAtk;
	}

	void Bulletremove();
	const std::list<std::unique_ptr<EnemyNormalBullet>>& GetBullets() { return Normalbullets_; };

	bool Death()
	{
		if (HP <= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	XMFLOAT3 GetPos()
	{
		return St->Wt.translation_;
	}
private:

	void AtkTable();

	void MoveTable();

	void BackMove();

	void FanShapeMove();

	void FlyMove();

	void SimpleShot();

	void ChargeAtk();

	void LaserShot();

	void MissileShot();

	void ImGuiSet();

	



private:
	//汎用
	XMFLOAT3 prePos = { 0,0,0 };
	Player* player;
	//攻撃パターン
	AtkPattern BossAtk = AtkPattern::NONE;

	//HP
	std::unique_ptr<Sprite2D> sprite_HPbar;

	uint32_t HpBarHandle;
	const float MaxHP = 100.0f;
	float HP = 0.0f;

	//行動パターン
	MovePattern BossMove = MovePattern::NONE;

	const int LongMoveTime = 60 * 6;

	const int MidMoveTime = 60 * 3;

	int MoveTimer = 0;

	float Angle=0;

	Draw3DLine LeserPoint;

	int WaitTimer = 0;
	const int WaitTime = 180;

	//弾関連
	std::list<std::unique_ptr<EnemyNormalBullet>> Normalbullets_;

	//照準周り
	XMFLOAT3 TargetPos = { 0,0,0 };

	const int TargetTime = 60 * 2;

	int TargetTimer = 0;

	bool AimMode = false;

	//通常射撃

	const int BurstRate = 5;
	const int BurstNum = 3;
	int BurstTime = BurstNum * BurstRate;

	//突進攻撃
	XMFLOAT3 TargetVec= { 0,0,0 };
	float chargeLenge = 0;
	int chargeCool = 0;
	const int chargeCoolTime = 600;


	//認知範囲
	float Lange = 0;
	float LangeMax = 50;
	float LangeMin = 10;

	

};

