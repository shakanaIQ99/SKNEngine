#pragma once
#include"Input.h"
#include"PlayerBullet.h"
#include"StuructTransform.h"


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

	void Draw();

	void DrawUI();

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
	XMFLOAT3 GetUnderPos()
	{
		XMFLOAT3 UnderPos = GetPos();
		UnderPos.y -= St->Wt.scale_.y;
		return UnderPos;
	}

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };
private:

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack(XMFLOAT3 flont);

	void Move();

	void Jump();

	void Dash(XMFLOAT3 front);

	void EN();

	bool LockOn();

	bool ScLock(WorldTransform* prewt);

	XMFLOAT2 WorldToMonitor(XMFLOAT3 pos);


	void ImGuiSet();


private:
	WorldTransform* boss;
	
	float move_speed;
	XMFLOAT3 moveVec;

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

	//----------------

	XMFLOAT2 Lock2DPos = { 0,0 };

	//弾関連
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	const float MaxHP = 20.0f;
	float HP = 0.0f;


	int bulletCT = 0;

	const int MAXBulletCT = 5;

	const int firelate = 15;

	int latetime;

	//クイックダッシュ周り

	float dashspeed = 0.0f;

	const float DashSpeadNum = 5.0f;

	XMFLOAT3 DashVec = { 0,0,0 };

	bool DashFlag = false;

	int DashTimer = 0;
	const int DashTime = 20;

	//ブーストゲージ的な何か-------
	const float ENGaugeSize = 15.0f;

	int ENGauge = 0;

	const int ENMAXGauge = 1000;

	const int DashUseGauge = 200;

	const int JumpUseGauge = 100;

	bool UseEN = false;

	int RegenENCoolTimer = 0;

	const int RegenENCoolTime = 60;

	const int RegenEN = 3;

	const int RecoveryENGauge = 2;

	bool OverHeat = false;

	int OverHeatENCoolTimer = 0;

	const int OverHeatENCoolTime = 180;

	//--------

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	XMFLOAT3 VectorMat(XMFLOAT3 vector, XMMATRIX mat);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	XMFLOAT3 VectorMat(XMMATRIX mat, XMFLOAT3 vector);

};

const DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& v, float s);

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& v, float s);

const DirectX::XMFLOAT3 operator*(float s, const DirectX::XMFLOAT3& v);

const DirectX::XMFLOAT3 operator-= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);


