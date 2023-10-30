#pragma once
#include "StuructTransform.h"
using namespace DirectX;

enum struct Pattern
{
	STRAIGHT,
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
	void Initlize(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity, Pattern _mode = Pattern::SCATTER);

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

	//衝突を検出したら呼び出されるコールバック関数
	void Death();

private:

	static std::unique_ptr<ObjModel> Premodel;

	XMFLOAT3 romdom = { 0,0,0 };

	const float spead = 1.0f;

	//モデル
	//StuructTransform bullet;


	//テクスチャハンドル


	//速度
	XMFLOAT3 Velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

	Pattern mode = Pattern::SCATTER;

};

const DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& v, float s);

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& v, float s);

const DirectX::XMFLOAT3 operator*(float s, const DirectX::XMFLOAT3& v);