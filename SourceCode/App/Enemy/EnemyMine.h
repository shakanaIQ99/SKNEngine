#pragma once
#include "StuructTransform.h"
using namespace DirectX;

enum mineState
{
	WAIT,
	EXPLOSION,
	STOPEXPLOSION,
	END

};

class EnemyMine:public StuructTransform
{
public:

	EnemyMine();
	~EnemyMine();

	static void SetModel(ObjModel* _model);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	void Initlize(const Vector3& Position, const Vector3& Rota, const Vector3& Velocity);

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

	bool IsHit()const;

	//衝突を検出したら呼び出されるコールバック関数
	void Destoroy();

	void OnCol();



	Vector3 GetWorldPosition() { return St->Wt.translation_; }
	Vector3 GetScale() { return St->Wt.scale_; }
	Vector3 GetVec() { return velocity; }




private:

	static std::unique_ptr<ObjModel> preModel;

	//モデル
	//StuructTransform bullet;


	//テクスチャハンドル


	//速度
	Vector3 velocity;

	//デスフラグ
	bool isDead = false;

	size_t mineTimer = 0;
	const size_t mineTime = 60;

	size_t explodeTimer = 0;
	const size_t explodeTime = 20;

	size_t stopCounter = 0;
	const size_t stopCount = 120;

	bool onField = false;

	mineState state = mineState::WAIT;

	float scale = 1.0f;

	bool hited = false;
	bool explode = false;

};

