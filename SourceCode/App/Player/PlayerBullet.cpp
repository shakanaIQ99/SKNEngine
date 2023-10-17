#include "PlayerBullet.h"

PlayerBullet::PlayerBullet()
{
	Velocity_ = { 0,0,0 };
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::SetModel(ObjModel* _model)
{
	ModelInit(_model);
}

void PlayerBullet::Initlize(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity)
{
	St->Wt.translation_ = position;
	St->Wt.rotation_ = rota;
	St->Wt.scale_ = { 0.5f,0.5f,0.5f };
	
	Velocity_ = velocity;
}

void PlayerBullet::Update()
{
	St->Wt.translation_ = St->Wt.translation_ + Velocity_;

	St->Update(camera->getView());

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void PlayerBullet::Draw()
{
	//bullet.Draw();
	St->Draw();
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

XMFLOAT3 PlayerBullet::GetWorldPosition()
{
	return St->Wt.translation_;
}

XMFLOAT3 PlayerBullet::GetScale()
{
	return St->Wt.scale_;
}
