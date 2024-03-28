#include "EnemyMine.h"
#include <random>
#include "Field.h"
#include"Easing.h"
#include"AudioManager.h"

std::unique_ptr<ObjModel> EnemyMine::Premodel;


EnemyMine::EnemyMine()
{

	ModelInit(Premodel.get());
	Velocity_ = { 0,0,0 };
}

EnemyMine::~EnemyMine()
{
}

void EnemyMine::SetModel(ObjModel* _model)
{
	Premodel.reset(_model);
}

void EnemyMine::Initlize(const Vector3& position, const Vector3& rota, const Vector3& velocity)
{
	St->Wt.translation_ = position;
	St->Wt.rotation_ = rota;
	St->Wt.scale_ = { scale,scale,scale };
	St->color = { 1.0f,0.6f,0.0f,1.0f };

	mineTimer = mineTime;
	stopCounter = stopCount;
	explodeTimer = 0;
	Velocity_ = velocity;

	Velocity_.y = -0.2f;
}

void EnemyMine::Update()
{

	

	if (St->Wt.translation_.y <= 0)
	{
		St->Wt.translation_.y = 0;
		onField = true;
		
	}
	
	if(!onField)
	{
		St->Wt.translation_ = St->Wt.translation_ + Velocity_;
	}
	
	if (onField)
	{
		switch (state)
		{
		case WAIT:
			mineTimer--;
			if (mineTimer <= 0)
			{
				state = mineState::EXPLOSION;
				explode = true;
				AudioManager::Play("mine");
			}
			break;
		case EXPLOSION:
			explodeTimer++;
			scale = easeOutQuint(1.0f, 4.0f, static_cast<float>(explodeTimer), static_cast<float>(explodeTime));

			
			if (explodeTimer >= explodeTime)state = mineState::STOPEXPLOSION;
			break;
		case STOPEXPLOSION:
			stopCounter--;
			if (stopCounter <= 0)
			{
				state = mineState::END;
				explodeTimer = 0;
			}
			break;
		case END:
			explodeTimer++;
			scale = easeInQuint(4.0f, 0.0f, static_cast<float>(explodeTimer), static_cast<float>(explodeTime));

			if (explodeTimer >= explodeTime)isDead_ = true;
			break;
		}
		
	}

	St->Wt.scale_ = { scale,scale,scale };


	St->Update(camera->getView());

	
}

void EnemyMine::Draw()
{
	St->Draw();
}

bool EnemyMine::IsHit() const
{
	if (!hited && explode)
	{
		return true;
	}
	return false;
}

void EnemyMine::Destoroy()
{
	isDead_ = true;
}

void EnemyMine::OnCol()
{
	hited = true;
}
