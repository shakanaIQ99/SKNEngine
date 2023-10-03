#include "BossEnemy.h"
#include"ImGuiManager.h"

#include <iostream>     // cout
#include <ctime>        // time
#include <cstdlib>      // srand,rand

void BossEnemy::Init()
{
	ModelInit("Player");
	EnemyNormalBullet::SetModel(ObjModel::LoadFromOBJ("maru"));
	St->Wt.scale_ = { 4.0f,4.0f,4.0f };
	St->color = { 1.0f,0,0,1.0f };
	LeserPoint.Init();
	HP = MaxHP;

	HpBarHandle = texMana->LoadTexture("Resources/HpBar.png");

	sprite_HPbar = std::make_unique<Sprite2D>();
	sprite_HPbar->Initialize(spCommon, HpBarHandle);
	sprite_HPbar->Wt.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 22.5f ,0.0f };
	sprite_HPbar->Wt.scale_.x = 30.0f;
	sprite_HPbar->Wt.color = { 1.0f,0.0f,0.0f,1.0f };

}

void BossEnemy::Reset()
{
	LeserPoint.Init();
	St->Wt.translation_ = { 0,0.0f,20.0f };
	HP = MaxHP;
	const std::list<std::unique_ptr<EnemyNormalBullet>>& Bullets = GetBullets();
	for (const std::unique_ptr<EnemyNormalBullet>& bullet : Bullets)
	{

		bullet->OnCollision();

	}
	Normalbullets_.remove_if([](std::unique_ptr<EnemyNormalBullet>& bullet)
		{
			return bullet->IsDead();
		});

}

void BossEnemy::Update()
{
	Normalbullets_.remove_if([](std::unique_ptr<EnemyNormalBullet>& bullet)
		{
			return bullet->IsDead();
		});

	//•½–Êã‚Ì‹——£
	XMFLOAT3 plUnderPos = player->GetUnderPos() - St->Wt.translation_;
	Lange = length(plUnderPos);

	sprite_HPbar->Wt.scale_.x = (30.0f * HP / MaxHP);

	St->Wt.translation_.y -= 0.5f;

	if (St->Wt.translation_.y - (St->Wt.scale_.y * 1.5f) < 0.0f)
	{
		St->Wt.translation_.y = (St->Wt.scale_.y * 1.5f);
	}

	chargeCool--;

	if (BossAtk != AtkPattern::CHARGE)
	{
		switch (BossMove)
		{
		case MovePattern::NONE:
			MoveTable();
			break;
		case MovePattern::BACK:
			BackMove();
			break;
		case MovePattern::FANSHAPE:
			FanShapeMove();
			break;
		case MovePattern::FLY:
			FlyMove();
			break;
		}
	}


	switch (BossAtk)
	{
		case AtkPattern::NONE:

			AimMode = false;
			WaitTimer = WaitTimer;
			AtkTable();

			break;
		case AtkPattern::SIMPLESHOT:

			SimpleShot();

			break;
		case AtkPattern::CHARGE:
			
			
			ChargeAtk();
			break;
		case AtkPattern::LASER:
			break;
		case AtkPattern::MISSILE:
			break;
	}

	for (std::unique_ptr<EnemyNormalBullet>& bullet : Normalbullets_)
	{
		bullet->Update();
	}

	ImGuiSet();

	St->Update(camera->getView());
	sprite_HPbar->Update();
}

void BossEnemy::Damege(float dmg)
{
	HP -= dmg;
}

void BossEnemy::Draw()
{
	for (std::unique_ptr<EnemyNormalBullet>& bullet : Normalbullets_)
	{
		bullet->Draw();
	}

	St->Draw();
	XMFLOAT3 Head = St->Wt.translation_;

	Head.y += St->Wt.scale_.y;
	if (AimMode)
	{
		LeserPoint.Draw(Head,TargetPos);
	}
}

void BossEnemy::DrawUI()
{
	if (HP > 0)
	{
		sprite_HPbar->Draw();
	}
}

void BossEnemy::AtkTable()
{
	if (WaitTimer < 0)
	{
		if (Lange > LangeMax)
		{
			TargetTimer = TargetTime;
			BossAtk = AtkPattern::SIMPLESHOT;
			BurstTime = BurstNum * BurstRate;

		}
		if (Lange < LangeMax)
		{
			if (chargeCool < 0)
			{
				BossAtk = AtkPattern::CHARGE;
				prePos = St->Wt.translation_;
				prePos.y = 0;
				TargetVec = player->GetPos() - St->Wt.translation_;
				TargetVec.y = 0;
				chargeLenge = length(TargetVec);
				normalize(TargetVec);
				chargeCool = chargeCoolTime;
			}


		}

	}
}

void BossEnemy::MoveTable()
{
	int MoveRand = rand() % 5;
	bool TimeRand = rand() % 1;
	if (Lange > LangeMax)
	{
		if (MoveRand<2)
		{
			BossMove = MovePattern::BACK;
		}
		else
		{
			BossMove = MovePattern::FANSHAPE;
		}
	}
	else
	{
		BossMove = MovePattern::BACK;
	}
	if (TimeRand)
	{
		MoveTimer = LongMoveTime;
	}
	else
	{
		MoveTimer = MidMoveTime;
	}

}

void BossEnemy::BackMove()
{
	XMFLOAT3 moveVec = St->Wt.translation_ - player->GetPos();
	moveVec.y = 0;

	normalize(moveVec);

	moveVec *= 0.2f;

	St->Wt.translation_ += moveVec;

	MoveTimer--;
	if (MoveTimer < 0) { BossMove = MovePattern::NONE; }
}

void BossEnemy::FanShapeMove()
{
	
	MoveTimer--;
	if (MoveTimer < 0) { BossMove = MovePattern::NONE; }
}

void BossEnemy::FlyMove()
{
}

void BossEnemy::SimpleShot()
{
	if (TargetTimer > 0)
	{
		if (TargetTimer > 10)
		{
			TargetPos = player->GetPos();

		}
		TargetTimer--;
		AimMode = true;
	}
	else
	{
		XMFLOAT3 Head = St->Wt.translation_;

		Head.y += St->Wt.scale_.y;
		AimMode = false;
		XMFLOAT3 BulletVec = TargetPos - Head;
		normalize(BulletVec);

		BulletVec *= 2.0f;

		if (BurstTime % BurstRate == 0)
		{
			std::unique_ptr <EnemyNormalBullet> newBullet = std::make_unique<EnemyNormalBullet>();
			newBullet->Initlize(Head, St->Wt.rotation_, BulletVec);

			Normalbullets_.push_back(std::move(newBullet));

		}
		BurstTime--;
		if (BurstTime <= 0)
		{
			BossAtk = AtkPattern::NONE;

		}
	}

}

void BossEnemy::ChargeAtk()
{
	

	XMFLOAT3 chargeMoved = St->Wt.translation_ - prePos;
	chargeMoved.y = 0;

	St->Wt.translation_ += TargetVec * 1.0f;

	if (chargeLenge + 5.0f < length(chargeMoved))
	{
		BossAtk = AtkPattern::NONE;
	}


}

void BossEnemy::LaserShot()
{
}

void BossEnemy::MissileShot()
{
}

void BossEnemy::ImGuiSet()
{
	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x + 400, ImGui::GetMainViewport()->WorkPos.y + 10 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Boss", NULL, window_flags);

	ImGui::Text("Position");
	ImGui::DragFloat("X", &St->Wt.translation_.x, 0.5f);
	ImGui::DragFloat("Y", &St->Wt.translation_.y, 0.5f);
	ImGui::DragFloat("Z", &St->Wt.translation_.z, 0.5f);
	ImGui::NewLine();
	ImGui::Text("Lange::%5.2f", Lange);
	ImGui::DragFloat("Min", &LangeMin, 0.5f);
	ImGui::DragFloat("Max", &LangeMax, 0.5f);
	ImGui::NewLine();
	static int AtkmodeNum = 0;
	const char* AtkModes[] = { "NONE", "SIMPLESHOT", "CHARGE","LASER","MISSILE" };
	ImGui::Combo("##AtkmodeNumCombo", &AtkmodeNum, AtkModes, IM_ARRAYSIZE(AtkModes));
	ImGui::SameLine();
	if (ImGui::Button("Change"))
	{
		switch (AtkmodeNum)
		{
		case 0:
			BossAtk = AtkPattern::NONE;
			break;
		case 1:
			TargetTimer = TargetTime;
			BossAtk = AtkPattern::SIMPLESHOT;
			BurstTime = BurstNum * BurstRate;
			break;
		case 2:
			BossAtk = AtkPattern::CHARGE;
			break;
		case 3:
			BossAtk = AtkPattern::LASER;
			break;
		case 4:
			BossAtk = AtkPattern::MISSILE;
			break;
		}

	}
	ImGui::Text("BossMode::%s", AtkModes[static_cast<int>(BossAtk)]);

	ImGui::NewLine();
	static int MovemodeNum = 0;
	const char* MoveModes[] = { "NONE", "BACK", "FANSHAPE" };
	ImGui::Combo("##MovemodeNumCombo", &MovemodeNum, MoveModes, IM_ARRAYSIZE(MoveModes));
	ImGui::SameLine();
	if (ImGui::Button("Change"))
	{
		switch (MovemodeNum)
		{
		case 0:
			BossMove = MovePattern::NONE;
			break;
		case 1:
			BossMove = MovePattern::BACK;
			break;
		case 2:
			BossMove = MovePattern::FANSHAPE;
			break;
		}

	}
	ImGui::Text("BossMode::%s", MoveModes[static_cast<int>(BossMove)]);
	ImGui::NewLine();
	ImGui::Text("HP::%5.2f", HP);
	ImGui::DragFloat("HP", &HP, 0.2f);

	ImGui::End();
}

void BossEnemy::Bulletremove()
{
	Normalbullets_.remove_if([](std::unique_ptr<EnemyNormalBullet>& bullet)
		{
			return bullet->IsDead();
		});

}
