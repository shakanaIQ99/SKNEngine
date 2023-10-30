#include "DeathParticle.h"
#include <iostream>
#include <random>
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

void DeathParticle::Initlize(const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity, Pattern _mode)
{
	St->Wt.translation_ = position;
	St->Wt.rotation_ = rota;
	St->Wt.scale_ = { 0.2f,0.2f,0.2f };
	St->Wt.color = { 0,0.3f,1.0f,1.0f };

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::default_random_engine eng2(rd());
	std::default_random_engine eng3(rd());
	std::uniform_real_distribution<float> distr(-180.0f, 180.0f);

	romdom = { distr(eng),distr(eng2),distr(eng3) };

	normalize(romdom);
	romdom *= spead;

	mode = _mode;

	Velocity_ = velocity;
}

void DeathParticle::Update()
{
	switch (mode)
	{
	case Pattern::STRAIGHT:


		break;
	case Pattern::SCATTER:


		St->Wt.translation_ = St->Wt.translation_ + romdom;
		break;
	}

	St->Update(camera->getView());

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
