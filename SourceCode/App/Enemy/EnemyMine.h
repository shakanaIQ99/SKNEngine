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
	void Initlize(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw();

	bool IsDead()const { return isDead_; }

	bool IsHit()const;

	//衝突を検出したら呼び出されるコールバック関数
	void Destoroy();

	void OnCol();



	XMFLOAT3 GetWorldPosition() { return St->Wt.translation_; }
	XMFLOAT3 GetScale() { return St->Wt.scale_; }
	XMFLOAT3 GetVec() { return Velocity_; }




private:

	static std::unique_ptr<ObjModel> Premodel;

	//モデル
	//StuructTransform bullet;


	//テクスチャハンドル


	//速度
	XMFLOAT3 Velocity_;

	//デスフラグ
	bool isDead_ = false;

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

