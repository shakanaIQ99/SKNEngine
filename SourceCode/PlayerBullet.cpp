#include "PlayerBullet.h"

PlayerBullet::PlayerBullet()
{
	bullet_model.reset(ObjModel::LoadFromOBJ("maru"));

	bullet.reset(OBJ3D::Create(&transform));
	bullet->SetModel(bullet_model.get());
	Velocity_ = { 0,0,0 };
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initlize(Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity)
{
	
	this->camera = camera;
	transform.translation_ = position;
	transform.rotation_ = rota;
	transform.scale_ = { 0.5f,0.5f,0.5f };
	
	Velocity_ = velocity;
}

void PlayerBullet::Update()
{
	transform.translation_ = transform.translation_ + Velocity_;

	bullet->Update(camera->getView());

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void PlayerBullet::Draw()
{
	//bullet.Draw();
	bullet->Draw();
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

XMFLOAT3 PlayerBullet::GetWorldPosition()
{
	return transform.translation_;
}

XMFLOAT3 PlayerBullet::GetScale()
{
	return transform.scale_;
}
