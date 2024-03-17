#pragma once
#include"Input.h"
#include"BulletManager.h"
#include"StuructTransform.h"
#include"ParticleManager.h"
#include"DeathParticle.h"


class Player : public StuructTransform
{
public:
	void SetEnemy(WorldTransform* enemy)
	{
		boss = enemy;
	}

	void Init();

	void Reset();

	void Update();

	void Damege(float dmg);

	void HitParticle(Vector3 vec);

	void KnockBack(Vector3 vec);

	void Draw();

	void DrawUI();

	/// <summary>
	/// 仮タイトル用更新
	/// </summary>
	void TitleUpdate();

	void StartUpdate();

	bool Start()
	{
		return startFlag;
	}

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

	bool GameEnd()
	{
		return endFlag;
	}

	Vector3 GetPos()
	{
		return St->Wt.translation_;
	}

	WorldTransform GetPre() 
	{
		return prePlayer;
	}
	Vector3 GetUnderPos()
	{
		Vector3 UnderPos = GetPos();
		UnderPos.y -= St->Wt.scale_.y;
		return UnderPos;
	}

	Vector3 GetPredictionPoint() { return playerPredictionPoint; }

	const std::list<std::unique_ptr<DeathParticle>>& GetDps() { return deathPaticles; };

	WorldTransform prePlayer;
private:

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack(Vector3 flont);

	void Move();

	void Jump(Vector3 front);

	void Dash(Vector3 front);

	void EN();

	void LockOn();

	bool ScLock(WorldTransform* prewt);

	void DeathAnimetion();

	Vector2 WorldToMonitor(Vector3 pos);


	void ImGuiSet();

	
	

private:
	WorldTransform* boss;

	std::unique_ptr<OBJ3D> prePP;

	std::unique_ptr<ObjModel> bulletModel;

	std::unique_ptr<OBJ3D> colBox;
	bool colLock = false;

	
	float move_speed;

	bool BoostMode = false;

	Vector3 moveVec;

	Vector3 playerPredictionPoint;

	Vector3 rotaVec = { 0,0,0 };

	Vector3 mae = { 0,0,0 };

	//HUD周り----------
	std::unique_ptr<Sprite2D> sprite_Reticle;
	std::unique_ptr<Sprite2D> sprite_Lock;
	std::unique_ptr<Sprite2D> sprite_HPbar;
	std::unique_ptr<Sprite2D> sprite_CoverHPbar;
	std::unique_ptr<Sprite2D> sprite_ENGauge;
	std::unique_ptr<Sprite2D> sprite_CoverENGaugebar;

	std::unique_ptr<Sprite2D> KeyUI;

	uint32_t reticleHandle;
	uint32_t LockHandle;
	uint32_t HpBarHandle;


	Vector2 hpBarShakePos = {0,0};

	float hpBarShakeNum = 0;

	//----------------

	Vector2 Lock2DPos = { 0,0 };

	float scale = 1.0f;
	int DpRate = 0;
	const int DpRateNum = 1;

	std::list<std::unique_ptr<DeathParticle>> deathPaticles;

	const float MaxHP = 20.0f;
	float HP = 0.0f;


	int bulletCT = 0;

	const int MAXBulletCT = 5;

	const int firelate = 15;

	int latetime;

	bool Locked = false;

	//クイックダッシュ周り

	float dashspeed = 0.0f;

	const float DashSpeadNum = 5.0f;

	Vector3 DashVec = { 0,0,0 };

	bool DashFlag = false;

	int DashTimer = 0;
	const int DashTime = 20;

	//ジャンプand空中周り

	bool OnGround = false;

	float Upspeed = 0.0f;

	Vector3 JumpVec = { 0,0,0 };

	const float AirUpSpead = 0.5f;

	const float UpSpeadNum = 2.0f;

	bool JumpFlag = false;

	int JumpTimer = 0;
	const int JumpTime = 20;

	//ブーストゲージ的な何か-------
	const float ENGaugeSize = 15.0f;

	int ENGauge = 0;

	const int ENMAXGauge = 1000;

	const int DashUseGauge = 200;

	const int JumpUseGauge = 100;

	const int AirUseGauge = 7;

	bool UseEN = false;

	int RegenENCoolTimer = 0;

	const int RegenENCoolTime = 60;

	const int RegenEN = 3;

	const int RecoveryENGauge = 1;

	bool OverHeat = false;

	int OverHeatENCoolTimer = 0;

	const int OverHeatENCoolTime = 250;

	bool InfEN = false;

	bool Muteki = false;



	//--------

	int DeathTimer = 0;

	const int DeathTime = 120;

	int SceneCameraTimer = 0;

	const int SceneCameraTime = 120;

	bool startFlag = false;

	bool endFlag = false;

	
	float diff = 0;

	//ノックバック
	bool knockBack = false;
	Vector3 knockVec = { 0,0,0 };
	float knockSpeed = 0;
	const float knockSpeedNum = 5.0f;
	

};


