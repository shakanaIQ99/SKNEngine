#pragma once
#include "StuructTransform.h"
using namespace DirectX;

enum struct Pattern
{
	STRAIGHT,
	HIT,
	SCATTER
};

class DeathParticle :public StuructTransform
{

public:
	DeathParticle();
	~DeathParticle();

	static void SetModel(ObjModel* model);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	void CreateDeathParticle(const Vector3& position, const Vector3& rota, const Vector3& velocity,float _scale,Float4 color);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	void CreateHitParticle(const Vector3& position, const Vector3& rota, const Vector3& velocity, float _scale, Float4 color);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw();

	bool IsDead()const { return isDead; }

	//衝突を検出したら呼び出されるコールバック関数
	void Death();

private:

	static std::unique_ptr<ObjModel> Premodel;

	Vector3 romdom = { 0,0,0 };

	const float spead = 0.3f;

	float startScale = 0;

	float scale = 0;

	//モデル
	//StuructTransform bullet;


	//テクスチャハンドル


	//速度
	Vector3 Velocity_;

	//寿命<frm>
	int32_t lifeTime = 60;

	//デスタイマー
	int32_t deathTimer = lifeTime;

	//デスフラグ
	bool isDead = false;

	Pattern mode = Pattern::SCATTER;

};

