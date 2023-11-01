#pragma once
//プレイヤー弾
#include "StuructTransform.h"
using namespace DirectX;
class PlayerBullet:public StuructTransform
{
public:
	PlayerBullet();
	~PlayerBullet();

	/// <summary>
	/// モデルセット
	/// </summary>
	/// <param name="model"></param>
	void SetModel(ObjModel* _model);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	void Initlize(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& _velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw();

	/// <summary>
	/// 生きてるか
	/// </summary>
	/// <returns></returns>
	bool IsDead()const { return isDead; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetWorldPosition();

	/// <summary>
	/// スケール取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetScale();

private:


	//モデル
	//StuructTransform bullet;
	

	//テクスチャハンドル


	//速度
	XMFLOAT3 velocity;

	//寿命<frm>
	static const int32_t LifeTime = 60 * 5;

	//デスタイマー
	int32_t DeathTimer = LifeTime;

	//デスフラグ
	bool isDead = false;

};
