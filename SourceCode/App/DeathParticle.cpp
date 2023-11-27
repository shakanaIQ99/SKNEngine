#include "DeathParticle.h"
#include <iostream>
#include <random>
#include"Easing.h"
#include"Player.h"
#include"myMath.h"
std::unique_ptr<ObjModel> DeathParticle::Premodel;

DeathParticle::DeathParticle()
{
	ModelInit(Premodel.get());
	Velocity_ = { 0,0,0 };
}

DeathParticle::~DeathParticle()
{
}

void DeathParticle::SetModel(ObjModel* model)
{
	Premodel.reset(model);
}

void DeathParticle::CreateDeathParticle(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity, float _scale, XMFLOAT4 color)
{
	St->Wt.translation_ = position;
	St->Wt.rotation_ = rota;
	startScale = _scale;
	scale = 0;
	St->Wt.scale_ = { startScale,startScale,startScale };
	St->color =color;

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::default_random_engine eng2(rd());
	std::default_random_engine eng3(rd());
	std::uniform_real_distribution<float> distr(-180.0f, 180.0f);

	romdom = { distr(eng),distr(eng2),distr(eng3) };

	normalize(romdom);
	romdom *= spead;

	mode = Pattern::SCATTER;

	Velocity_ = velocity;
}

void DeathParticle::CreateHitParticle(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity, float _scale, XMFLOAT4 color)
{
	St->Wt.translation_ = position;
	St->Wt.rotation_ = rota;
	startScale = _scale;
	scale = 0;
	St->Wt.scale_ = { startScale,startScale,startScale };
	St->color = color;
	Velocity_ = velocity;
	Velocity_.y = 0;
	normalize(Velocity_);

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::default_random_engine eng2(rd());
	std::default_random_engine eng3(rd());
	std::uniform_real_distribution<float> distr(-60.0f, 60.0f);

	romdom = { distr(eng),distr(eng2),distr(eng3) };

	XMMATRIX rotaMat;

	rotaMat = XMMatrixIdentity();

	rotaMat *= XMMatrixRotationY(XMConvertToRadians(distr(eng)));
	rotaMat *= XMMatrixRotationX(XMConvertToRadians(distr(eng2)));

	mode = Pattern::HIT;
	kLifeTime = 30;
	deathTimer_ = kLifeTime;

	Velocity_ = myMath::VectorMat(Velocity_, rotaMat);

	normalize(Velocity_);
	romdom *= spead;
	Velocity_ *= (spead*2);
}

void DeathParticle::Update()
{
	switch (mode)
	{
	case Pattern::STRAIGHT:


		break;

	case Pattern::HIT:
		scale = easeInSine(0, startScale, static_cast<float>(deathTimer_), static_cast<float>(kLifeTime));
		//St->color.z = easeInSine(0, 1.0f, static_cast<float>(deathTimer_), static_cast<float>(kLifeTime));
		St->Wt.translation_ = St->Wt.translation_ + Velocity_;
		St->Wt.rotation_ = St->Wt.rotation_ + romdom;
		St->Wt.scale_ = { scale,scale,scale };

		break;
	case Pattern::SCATTER:

		scale = easeInSine(0, startScale, static_cast<float>(deathTimer_), static_cast<float>(kLifeTime));
		St->color.z= easeInSine(0, 1.0f, static_cast<float>(deathTimer_), static_cast<float>(kLifeTime));
		St->Wt.translation_ = St->Wt.translation_ + romdom;
		St->Wt.rotation_ = St->Wt.rotation_ + romdom;
		St->Wt.scale_ = { scale,scale,scale };
		break;
	}

	St->Update(camera->getView());
	if (deathTimer_ <= 0)
	{
		deathTimer_ = 0;
	}

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void DeathParticle::Draw()
{
	St->Draw();
}

void DeathParticle::Death()
{
	isDead_ = true;
}
