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

	void Initialize();

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
		if (hp <= 0)
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

	Vector3 GetMoveVec(bool norm = false)
	{
		if (norm)
		{
			return (mae - St->Wt.translation_).GetNormalize();
		}
		return mae - St->Wt.translation_;
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


	void ImGuiMode();

	
	

private:
	WorldTransform* boss;

	std::unique_ptr<OBJ3D> prePP;

	std::unique_ptr<ObjModel> bulletModel;

	std::unique_ptr<OBJ3D> colBox;
	bool colLock = false;

	
	float move_speed;

	bool boostMode = false;

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

	std::unique_ptr<Sprite2D> keyUI;

	Vector2 hpBarShakePos = {0,0};

	float hpBarShakeNum = 0;

	//----------------

	Vector2 Lock2DPos = { 0,0 };

	float scale = 1.0f;
	int dpRate = 0;
	const int dpRateNum = 1;

	std::list<std::unique_ptr<DeathParticle>> deathPaticles;

	const float maxhp = 20.0f;
	float hp = 0.0f;


	int bulletCT = 0;

	const int maxBulletCT = 5;

	const int fireLate = 15;

	int lateTime;

	bool locked = false;

	//クイックダッシュ周り

	float dashSpeed = 0.0f;

	const float dashSpeadNum = 5.0f;

	Vector3 dashVec = { 0,0,0 };

	bool dashFlag = false;

	int dashTimer = 0;
	const int dashTime = 20;

	//ジャンプand空中周り

	bool onGround = false;

	float upSpeed = 0.0f;

	Vector3 jumpVec = { 0,0,0 };

	const float airUpSpead = 0.5f;

	const float upSpeadNum = 2.0f;

	bool jumpFlag = false;

	int jumpTimer = 0;
	const int jumpTime = 20;

	//ブーストゲージ的な何か-------
	const float enGaugeSize = 15.0f;

	int gaugeEN = 0;

	const int maxGaugeEN = 1000;

	const int dashUseGauge = 200;

	const int jumpUseGauge = 100;

	const int airUseGauge = 7;

	bool useEN = false;

	int regenENCoolTimer = 0;

	const int regenENCoolTime = 60;

	const int regenEN = 3;

	const int recoveryENGauge = 1;

	bool overHeat = false;

	int overHeatENCoolTimer = 0;

	const int overHeatENCoolTime = 250;

	bool infEN = false;

	bool muteki = false;



	//--------

	int deathTimer = 0;

	const int deathTime = 120;

	int sceneCameraTimer = 0;

	const int sceneCameraTime = 120;

	bool startFlag = false;

	bool endFlag = false;

	
	float diff = 0;

	//ノックバック
	bool knockBack = false;
	Vector3 knockVec = { 0,0,0 };
	float knockSpeed = 0;
	const float knockSpeedNum = 5.0f;
	

};


