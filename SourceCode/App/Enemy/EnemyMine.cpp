#include "EnemyMine.h"
#include <random>
#include "Field.h"
#include"Easing.h"
#include"AudioManager.h"

std::unique_ptr<ObjModel> EnemyMine::preModel;


EnemyMine::EnemyMine()
{

	ModelInit(preModel.get());
	velocity = { 0,0,0 };
}

EnemyMine::~EnemyMine()
{
}

void EnemyMine::SetModel(ObjModel* _model)
{
	preModel.reset(_model);
}

void EnemyMine::Initlize(const Vector3& Position, const Vector3& Rota, const Vector3& Velocity)
{
	St->Wt.translation_ = Position;
	St->Wt.rotation_ = Rota;
	St->Wt.scale_ = { scale,scale,scale };
	St->color = { 1.0f,0.6f,0.0f,1.0f };

	mineTimer = mineTime;
	stopCounter = stopCount;
	explodeTimer = 0;
	velocity = Velocity;

	velocity.y = -0.2f;
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
		St->Wt.translation_ = St->Wt.translation_ + velocity;
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
			scale = EaseOutQuint(1.0f, 4.0f, static_cast<float>(explodeTimer), static_cast<float>(explodeTime));

			
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
			scale = EaseInQuint(4.0f, 0.0f, static_cast<float>(explodeTimer), static_cast<float>(explodeTime));

			if (explodeTimer >= explodeTime)isDead = true;
			break;
		}
		
	}

	St->Wt.scale_ = { scale,scale,scale };


	St->Update(camera->GetView());

	
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
	isDead = true;
}

void EnemyMine::OnCol()
{
	hited = true;
}
