#pragma once
#include "StuructTransform.h"
using namespace DirectX;

class EnemyNormalBullet:public StuructTransform
{
public:
	EnemyNormalBullet();
	~EnemyNormalBullet();

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

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	XMFLOAT3 GetWorldPosition() { return St->Wt.translation_; }
	XMFLOAT3 GetScale() { return St->Wt.scale_; }


private:

	static std::unique_ptr<ObjModel> Premodel;

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


};

