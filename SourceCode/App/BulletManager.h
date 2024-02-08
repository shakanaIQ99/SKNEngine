#pragma once
#include "StuructTransform.h"
using namespace DirectX;

class BulletManager:public StuructTransform
{
public:


	BulletManager();
	virtual ~BulletManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	virtual void Initlize(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	virtual void Draw();

	virtual bool IsDead()const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	virtual void OnCollision();

	XMFLOAT3 GetWorldPosition();
	XMFLOAT3 GetScale();
	XMFLOAT3 GetVec() { return Velocity_; }

private:


	//モデル
	//StuructTransform bullet;


	//テクスチャハンドル


	//速度
	XMFLOAT3 Velocity_;


	//デスフラグ
	bool isDead_ = false;





};

