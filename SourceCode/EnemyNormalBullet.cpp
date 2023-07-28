#include "EnemyNormalBullet.h"

std::unique_ptr<ObjModel> EnemyNormalBullet::Premodel;

EnemyNormalBullet::EnemyNormalBullet()
{
	ModelInit(Premodel.get());
	Velocity_ = { 0,0,0 };
}

EnemyNormalBullet::~EnemyNormalBullet()
{
}

void EnemyNormalBullet::SetModel(ObjModel* _model)
{
	Premodel.reset(_model);
}

void EnemyNormalBullet::Initlize(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity)
{
	transform.translation_ = position;
	transform.rotation_ = rota;
	transform.scale_ = { 0.5f,0.5f,0.5f };

	Velocity_ = velocity;
}

void EnemyNormalBullet::Update()
{
	transform.translation_ = transform.translation_ + Velocity_;

	St->Update(camera->getView());

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyNormalBullet::Draw()
{
	St->Draw();
}

void EnemyNormalBullet::OnCollision()
{
	isDead_ = true;
}
