#pragma once
#include"Input.h"
#include"PlayerBullet.h"
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

	void HitParticle(XMFLOAT3 vec);

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

	XMFLOAT3 GetPos()
	{
		return St->Wt.translation_;
	}

	WorldTransform GetPre() 
	{
		return prePlayer;
	}
	XMFLOAT3 GetUnderPos()
	{
		XMFLOAT3 UnderPos = GetPos();
		UnderPos.y -= St->Wt.scale_.y;
		return UnderPos;
	}

	XMFLOAT3 GetPredictionPoint() { return playerPredictionPoint; }

	const std::list<std::unique_ptr<BulletManager>>& GetBullets() { return bullets_; };

	const std::list<std::unique_ptr<DeathParticle>>& GetDps() { return deathPaticles; };

	WorldTransform prePlayer;
private:

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack(XMFLOAT3 flont);

	void Move();

	void Jump(XMFLOAT3 front);

	void Dash(XMFLOAT3 front);

	void EN();

	void LockOn();

	bool ScLock(WorldTransform* prewt);

	void DeathAnimetion();

	XMFLOAT2 WorldToMonitor(XMFLOAT3 pos);


	void ImGuiSet();

	
	

private:
	WorldTransform* boss;

	std::unique_ptr<OBJ3D> prePP;

	
	float move_speed;

	bool BoostMode = false;

	XMFLOAT3 moveVec;

	XMFLOAT3 playerPredictionPoint;

	XMFLOAT3 rotaVec = { 0,0,0 };

	XMFLOAT3 mae = { 0,0,0 };

	//HUD周り----------
	std::unique_ptr<Sprite2D> sprite_Reticle;
	std::unique_ptr<Sprite2D> sprite_Lock;
	std::unique_ptr<Sprite2D> sprite_HPbar;
	std::unique_ptr<Sprite2D> sprite_CoverHPbar;
	std::unique_ptr<Sprite2D> sprite_ENGauge;
	std::unique_ptr<Sprite2D> sprite_CoverENGaugebar;

	uint32_t reticleHandle;
	uint32_t LockHandle;
	uint32_t HpBarHandle;


	XMFLOAT2 hpBarShakePos = {0,0};

	float hpBarShakeNum = 0;

	//----------------

	XMFLOAT2 Lock2DPos = { 0,0 };

	float scale = 1.0f;
	int DpRate = 0;
	const int DpRateNum = 1;

	//弾関連
	std::list<std::unique_ptr<BulletManager>> bullets_;

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

	XMFLOAT3 DashVec = { 0,0,0 };

	bool DashFlag = false;

	int DashTimer = 0;
	const int DashTime = 20;

	//ジャンプand空中周り

	bool OnGround = false;

	float Upspeed = 0.0f;

	XMFLOAT3 JumpVec = { 0,0,0 };

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



	//--------

	int DeathTimer = 0;

	const int DeathTime = 120;

	int SceneCameraTimer = 0;

	const int SceneCameraTime = 120;

	bool startFlag = false;

	bool endFlag = false;


	float diff = 0;
	

};

const DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& v, float s);

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& v, float s);

const DirectX::XMFLOAT3 operator*(float s, const DirectX::XMFLOAT3& v);

const DirectX::XMFLOAT3 operator-= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& v1);




