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

void DeathParticle::CreateDeathParticle(const Vector3& position, const Vector3& rota, const Vector3& velocity, float _scale, Float4 color)
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

	romdom.Normalize();
	romdom *= spead;

	mode = Pattern::SCATTER;

	Velocity_ = velocity;
}

void DeathParticle::CreateHitParticle(const Vector3& position, const Vector3& rota, const Vector3& velocity, float _scale, Float4 color)
{
	St->Wt.translation_ = position;
	St->Wt.rotation_ = rota;
	startScale = _scale;
	scale = 0;
	St->Wt.scale_ = { startScale,startScale,startScale };
	St->color = color;
	Velocity_ = velocity;
	Velocity_.y = 0;

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::default_random_engine eng2(rd());
	std::default_random_engine eng3(rd());
	std::uniform_real_distribution<float> distr(-60.0f, 60.0f);

	romdom = { distr(eng),distr(eng2),distr(eng3) };

	Matrix4 rotaMat;

	

	rotaMat *= Matrix4::RotationY(distr(eng));
	rotaMat *= Matrix4::RotationX(distr(eng2));

	mode = Pattern::HIT;
	lifeTime = 30;
	deathTimer = lifeTime;

	Velocity_ = Velocity_ * rotaMat;

	Velocity_.Normalize();
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
		scale = EaseInSine(0, startScale, static_cast<float>(deathTimer), static_cast<float>(lifeTime));
		//St->color.z = easeInSine(0, 1.0f, static_cast<float>(deathTimer_), static_cast<float>(kLifeTime));
		St->Wt.translation_ = St->Wt.translation_ + Velocity_;
		St->Wt.rotation_ = St->Wt.rotation_ + romdom;
		St->Wt.scale_ = { scale,scale,scale };

		break;
	case Pattern::SCATTER:

		scale = EaseInSine(0, startScale, static_cast<float>(deathTimer), static_cast<float>(lifeTime));
		St->color.z= EaseInSine(0, 1.0f, static_cast<float>(deathTimer), static_cast<float>(lifeTime));
		St->Wt.translation_ = St->Wt.translation_ + romdom;
		St->Wt.rotation_ = St->Wt.rotation_ + romdom;
		St->Wt.scale_ = { scale,scale,scale };
		break;
	}

	St->Update(camera->GetView());
	if (deathTimer <= 0)
	{
		deathTimer = 0;
	}

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer <= 0)
	{
		isDead = true;
	}

}

void DeathParticle::Draw()
{
	St->Draw();
}

void DeathParticle::Death()
{
	isDead = true;
}
