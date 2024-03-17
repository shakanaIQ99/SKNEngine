//#pragma once
//#include "BulletManager.h"
//
//
//class EnemyNormalBullet:public BulletManager
//{
//public:
//	EnemyNormalBullet();
//	~EnemyNormalBullet();
//
//	static void SetModel(ObjModel* _model);
//
//	/// <summary>
//	/// 初期化
//	/// </summary>
//	/// <param name="model">モデル</param>
//	/// <param name="position">初期座標</param>
//	/// <param name="velocity">速度</param>
//	void Initlize(const Vector3& position, const Vector3& rota, const Vector3& velocity, const Tag& _tag = Tag::ENEMY)override;
//
//	/// <summary>
//	/// 更新
//	/// </summary>
//	void Update() override;
//
//	void Draw() override;
//
//	Vector3 GetWorldPosition()override { return St->Wt.translation_; }
//	Vector3 GetScale()override { return St->Wt.scale_; }
//	Vector3 GetVec() override { return Velocity_; }
//	Tag GetTag()override { return tag; }
//
//
//private:
//
//	Tag tag = Tag::NONE;
//
//	static std::unique_ptr<ObjModel> Premodel;
//
//	//モデル
//	//StuructTransform bullet;
//
//
//	//テクスチャハンドル
//
//
//	//速度
//	Vector3 Velocity_;
//
//
//
//};
//
