#pragma once
//敵
#include"StuructTransform.h"
#include"BulletManager.h"
#include"HomingBullet.h"
#include"EnemyMine.h"
#include"Draw3DLine.h"
#include"Player.h"
#include"DeathParticle.h"

enum struct AtkPattern
{
	NONE,
	SIMPLESHOT,
	CHARGE,
	HARDSHOT,
	MISSILE,
	MINE
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
	const std::list<std::unique_ptr<EnemyMine>>& GetMines() { return Mines_; };

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

	/// <summary>
	/// ゲームエンド時
	/// </summary>
	/// <returns></returns>
	bool GameEnd()
	{
		return endFlag;
	}

	/// <summary>
	/// 被弾時パーティクル
	/// </summary>
	/// <param name="vec"></param>
	void HitParticle(Vector3 vec);

	const std::list<std::unique_ptr<DeathParticle>>& GetDps() { return deathPaticles; };

	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetPos()
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
	/// 地雷
	/// </summary>
	void MineAttack();

	/// <summary>
	/// ImGui関係
	/// </summary>
	void ImGuiSet();

	void aiming();

	/// <summary>
	/// 死亡時の動き
	/// </summary>
	void DeathAnimetion();

	Vector3 LinePrediction2(Vector3 shotPosition, Vector3 targetPosition, Vector3 targetPrePosition, float bulletSpeed);

	float PlusMin(float a, float b)
	{
		if (a < 0 && b < 0) return 0;
		if (a < 0) return b;
		if (b < 0) return a;
		return a < b ? a : b;
	}

private:
	std::unique_ptr<ObjModel> bulletModel;
	std::unique_ptr<ObjModel> hbulletModel;

	std::unique_ptr<OBJ3D> colBox;
	bool colLock = false;

	//演出周り

	std::list<std::unique_ptr<DeathParticle>> deathPaticles;

	//--------

	Vector3 rotaVec = { 0,0,0 };

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
	Vector3 prePos = { 0,0,0 };
	Player* player;

	Vector3 aimingTargetPos = { 0,0,0 };
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

	std::list<std::unique_ptr<EnemyMine>> Mines_;

	const float nBulletSpeed = 2.0f;
	const float hBulletSpeed = 0.05f;

	//照準周り
	Vector3 TargetPos = { 0,0,0 };

	const int TargetTime = 60 * 2;

	int TargetTimer = 0;

	bool AimMode = false;

	//通常射撃

	const int BurstRate = 3;
	const int BurstNum = 40;
	int BurstTime = BurstNum * BurstRate;

	bool CriticalAim = true;

	//予測撃ち関係
	const size_t criAimTime = 30;
	size_t criAimTimer = 0;

	float crossLine = 0;


	//突進攻撃
	Vector3 TargetVec = { 0,0,0 };
	float chargeLenge = 0;
	int chargeCool = 0;
	const int chargeCoolTime = 600;

	int chargeMoveAniTimer = 0;

	const int chargeMoveAniTime = 30;

	//認知範囲
	float Lange = 0;
	float LangeMax = 5.0f;
	float LangeLong = 150.0f;
	float LangeMin = 10.0f;

	//地雷
	const size_t mineNum = 6;

	float mineThrowDeg = 0;

	size_t mineThrowTimer = 0;

	const size_t mineThrowTime = 40;

	int mineCool = 0;
	const int mineCoolTime = 300;

	

};