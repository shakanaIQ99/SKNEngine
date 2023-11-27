#pragma once
//敵
#include"StuructTransform.h"
#include"EnemyNormalBullet.h"
#include"Draw3DLine.h"
#include"Player.h"
#include"DeathParticle.h"

enum struct AtkPattern
{
	NONE,
	SIMPLESHOT,
	CHARGE,
	HARDSHOT,
	MISSILE
};

enum struct MovePattern
{
	NONE,
	BACK,
	CLOSEMOVE,
};

class BossEnemy : public StuructTransform
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossEnemy();

	/// <summary>
	/// プレイヤーポインタ
	/// </summary>
	/// <param name="pl"></param>
	void SetPlayer(Player* pl)
	{
		player = pl;
	}


	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool flag=false);

	/// <summary>
	/// 被ダメ
	/// </summary>
	/// <param name="dmg"></param>
	void Damege(float dmg);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 攻撃パターン取得
	/// </summary>
	/// <returns></returns>
	AtkPattern GetAtkPattern()
	{
		return BossAtk;
	}

	/// <summary>
	/// 弾削除
	/// </summary>
	void Bulletremove();
	const std::list<std::unique_ptr<EnemyNormalBullet>>& GetBullets() { return Normalbullets_; };

	/// <summary>
	/// 生きてるか
	/// </summary>
	/// <returns></returns>
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

	void HitParticle(XMFLOAT3 vec);

	const std::list<std::unique_ptr<DeathParticle>>& GetDps() { return deathPaticles; };

	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetPos()
	{
		return St->Wt.translation_;
	}
private:

	/// <summary>
	/// 攻撃テーブル
	/// </summary>
	void AtkTable();

	/// <summary>
	/// 移動テーブル
	/// </summary>
	void MoveTable();

	/// <summary>
	/// 後退
	/// </summary>
	void BackMove();

	/// <summary>
	/// 接近
	/// </summary>
	void CloseMove();

	/// <summary>
	/// 通常射撃
	/// </summary>
	void SimpleShot();

	/// <summary>
	/// 突進攻撃
	/// </summary>
	void ChargeAtk();

	/// <summary>
	/// 強射撃
	/// </summary>
	void HardShot();

	/// <summary>
	/// ミサイル
	/// </summary>
	void MissileShot();

	/// <summary>
	/// ImGui関係
	/// </summary>
	void ImGuiSet();


	void DeathAnimetion();


private:
	//演出周り

	std::list<std::unique_ptr<DeathParticle>> deathPaticles;

	//--------

	XMFLOAT3 rotaVec = { 0,0,0 };

	float scale = 4.0f;
	int DpRate = 0;
	const int DpRateNum = 1;

	int DeathTimer = 0;

	const int DeathTime = 120;

	int SceneCameraTimer = 0;

	const int SceneCameraTime = 120;

	bool startFlag = false;

	bool endFlag = false;

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

	float Angle = 0;

	Draw3DLine LeserPoint;

	int stopTimer = 0;
	const int stopTime = 120;

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

	const int BurstRate = 3;
	const int BurstNum = 40;
	int BurstTime = BurstNum * BurstRate;

	//突進攻撃
	XMFLOAT3 TargetVec = { 0,0,0 };
	float chargeLenge = 0;
	int chargeCool = 0;
	const int chargeCoolTime = 600;

	int chargeMoveAniTimer = 0;

	const int chargeMoveAniTime = 30;

	//認知範囲
	float Lange = 0;
	float LangeMax = 50;
	float LangeMin = 10;

	

};