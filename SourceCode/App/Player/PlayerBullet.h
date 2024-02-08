#pragma once
#include "BulletManager.h"
using namespace DirectX;
class PlayerBullet:public BulletManager
{
public:
	PlayerBullet();
	~PlayerBullet();

	static void SetModel(ObjModel* model);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	void Initlize(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw() override;

	
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
