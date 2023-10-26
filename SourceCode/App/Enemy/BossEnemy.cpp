#include "BossEnemy.h"
#include"ImGuiManager.h"
#include"Field.h"

#include <iostream>     // cout
#include <ctime>        // time
#include <cstdlib>      // srand,rand

BossEnemy::BossEnemy()
{
}

void BossEnemy::Init()
{
	ModelInit("Player");
	EnemyNormalBullet::SetModel(ObjModel::LoadFromOBJ("maru"));
	St->Wt.scale_ = { 4.0f,4.0f,4.0f };
	St->color = { 1.0f,0,0,1.0f };
	leserPoint.Init();
	HP = maxHP;

	hpBarHandle = texMana->LoadTexture("Resources/HpBar.png");

	sprite_HPbar = std::make_unique<Sprite2D>();
	sprite_HPbar->Initialize(spCommon, hpBarHandle);
	sprite_HPbar->Wt.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 22.5f ,0.0f };
	sprite_HPbar->Wt.scale_.x = 30.0f;
	sprite_HPbar->Wt.color = { 1.0f,0.0f,0.0f,1.0f };

}

void BossEnemy::Reset()
{
	leserPoint.Init();
	St->Wt.translation_ = { 0,0.0f,20.0f };
	HP = maxHP;
	const std::list<std::unique_ptr<EnemyNormalBullet>>& Bullets = GetBullets();
	for (const std::unique_ptr<EnemyNormalBullet>& bullet : Bullets)
	{

		bullet->OnCollision();

	}
	normalBullets.remove_if([](std::unique_ptr<EnemyNormalBullet>& bullet)
		{
			return bullet->IsDead();
		});

}

void BossEnemy::Update()
{
	normalBullets.remove_if([](std::unique_ptr<EnemyNormalBullet>& bullet)
		{
			return bullet->IsDead();
		});

	//平面上の距離
	XMFLOAT3 plUnderPos = player->GetUnderPos() - St->Wt.translation_;
	reactionLange = length(plUnderPos);

	sprite_HPbar->Wt.scale_.x = (30.0f * HP / maxHP);

	St->Wt.translation_.y -= 0.5f;

	if (St->Wt.translation_.y - (St->Wt.scale_.y * 1.5f) < 0.0f)
	{
		St->Wt.translation_.y = (St->Wt.scale_.y * 1.5f);
	}

	chargeCool--;

	if (bossAtk != AtkPattern::CHARGE)
	{
		switch (bossMove)
		{
		case MovePattern::NONE:

			stopTimer = stopTime;
			MoveTable();
			break;
		case MovePattern::BACK:
			BackMove();
			break;
		case MovePattern::CLOSEMOVE:
			CloseMove();
			break;
		}
	}


	switch (bossAtk)
	{
		case AtkPattern::NONE:

			aimMode = false;
			waitTimer = waitTimer;
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

	for (std::unique_ptr<EnemyNormalBullet>& bullet : normalBullets)
	{
		bullet->Update();
	}

#ifdef _DEBUG
	ImGuiSet();
#endif

	if (St->Wt.translation_.x + St->Wt.scale_.x > Field::GetArea() || St->Wt.translation_.x - St->Wt.scale_.x < -Field::GetArea())
	{
		St->Wt.translation_.x = Field::GetArea() - St->Wt.scale_.x * (abs(St->Wt.translation_.x) / St->Wt.translation_.x);
	}
	if (St->Wt.translation_.z + St->Wt.scale_.z >= Field::GetArea() || St->Wt.translation_.z - St->Wt.scale_.z <= -Field::GetArea())
	{
		St->Wt.translation_.z = Field::GetArea() - St->Wt.scale_.z * (abs(St->Wt.translation_.z) / St->Wt.translation_.z);
	}

	St->Update(camera->getView());
	sprite_HPbar->Update();
}

void BossEnemy::Damege(float dmg)
{
	HP -= dmg;
}

void BossEnemy::Draw()
{
	for (std::unique_ptr<EnemyNormalBullet>& bullet : normalBullets)
	{
		bullet->Draw();
	}

	St->Draw();
	XMFLOAT3 Head = St->Wt.translation_;

	Head.y += St->Wt.scale_.y;
	if (aimMode)
	{
		leserPoint.Draw(Head,targetPos);
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
	if (waitTimer < 0)
	{
		if (reactionLange > reactionLangeMax)
		{
			targetTimer = targetTime;
			bossAtk = AtkPattern::SIMPLESHOT;
			burstTime = burstNum * burstRate;

		}
		if (reactionLange < reactionLangeMax)
		{
			if (chargeCool < 0)
			{
				bossAtk = AtkPattern::CHARGE;
				prePos = St->Wt.translation_;
				prePos.y = 0;
				targetVec = player->GetPos() - St->Wt.translation_;
				targetVec.y = 0;
				chargeLenge = length(targetVec);
				normalize(targetVec);
				chargeCool = chargeCoolTime;
			}


		}

	}

	//WaitTimer--;
}

void BossEnemy::MoveTable()
{
	if (stopTimer < 0)
	{
		int MoveRand = rand() % 5;
		bool TimeRand = rand() % 1;
		if (reactionLange > reactionLangeMax)
		{
			if (MoveRand < 2)
			{
				bossMove = MovePattern::BACK;
			}
			else
			{
				bossMove = MovePattern::CLOSEMOVE;
			}
		}
		else
		{
			bossMove = MovePattern::BACK;
		}

		if (TimeRand)
		{
			moveTimer = longMoveTime;
		}
		else
		{
			moveTimer = midMoveTime;
		}
	}

	stopTimer--;

}

void BossEnemy::BackMove()
{
	XMFLOAT3 moveVec = St->Wt.translation_ - player->GetPos();
	moveVec.y = 0;

	normalize(moveVec);

	moveVec *= 0.2f;

	St->Wt.translation_ += moveVec;

	moveTimer--;
	if (moveTimer < 0) { bossMove = MovePattern::NONE; }
}

void BossEnemy::CloseMove()
{
	XMFLOAT3 moveVec = player->GetPos() - St->Wt.translation_;
	moveVec.y = 0;

	normalize(moveVec);

	moveVec *= 0.2f;

	St->Wt.translation_ += moveVec;


	moveTimer--;
	if (moveTimer < 0) { bossMove = MovePattern::NONE; }
}


void BossEnemy::SimpleShot()
{
	if (targetTimer > 0)
	{
		if (targetTimer > 10)
		{
			targetPos = player->GetPos();

		}
		targetTimer--;
		aimMode = true;
	}
	else
	{
		XMFLOAT3 Head = St->Wt.translation_;

		Head.y += St->Wt.scale_.y;
		aimMode = false;
		XMFLOAT3 BulletVec = player->GetPos() - Head;
		normalize(BulletVec);

		BulletVec *= 2.0f;

		if (burstTime % burstRate == 0)
		{
			std::unique_ptr <EnemyNormalBullet> newBullet = std::make_unique<EnemyNormalBullet>();
			newBullet->Initlize(Head, St->Wt.rotation_, BulletVec);

			normalBullets.push_back(std::move(newBullet));

		}
		burstTime--;
		if (burstTime <= 0)
		{
			bossAtk = AtkPattern::NONE;

		}
	}

}

void BossEnemy::ChargeAtk()
{
	

	XMFLOAT3 chargeMoved = St->Wt.translation_ - prePos;
	chargeMoved.y = 0;

	St->Wt.translation_ += targetVec * 1.0f;

	if (chargeLenge + 5.0f < length(chargeMoved))
	{
		bossAtk = AtkPattern::NONE;
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
	ImGui::Text("Lange::%5.2f", reactionLange);
	ImGui::DragFloat("Min", &reactionLangeMin, 0.5f);
	ImGui::DragFloat("Max", &reactionLangeMax, 0.5f);
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
			bossAtk = AtkPattern::NONE;
			break;
		case 1:
			targetTimer = targetTime;
			bossAtk = AtkPattern::SIMPLESHOT;
			burstTime = burstNum * burstRate;
			break;
		case 2:
			bossAtk = AtkPattern::CHARGE;
			break;
		case 3:
			bossAtk = AtkPattern::LASER;
			break;
		case 4:
			bossAtk = AtkPattern::MISSILE;
			break;
		}

	}
	ImGui::Text("BossMode::%s", AtkModes[static_cast<int>(bossAtk)]);

	ImGui::NewLine();
	static int MovemodeNum = 0;
	const char* MoveModes[] = { "NONE", "BACK", "CLOSEMOVE" };
	ImGui::Combo("##MovemodeNumCombo", &MovemodeNum, MoveModes, IM_ARRAYSIZE(MoveModes));
	ImGui::SameLine();
	if (ImGui::Button("Change"))
	{
		switch (MovemodeNum)
		{
		case 0:
			bossMove = MovePattern::NONE;
			break;
		case 1:
			bossMove = MovePattern::BACK;
			break;
		case 2:
			bossMove = MovePattern::CLOSEMOVE;
			break;
		}

	}
	ImGui::Text("BossMode::%s", MoveModes[static_cast<int>(bossMove)]);
	ImGui::NewLine();
	ImGui::Text("HP::%5.2f", HP);
	ImGui::DragFloat("HP", &HP, 0.2f);

	ImGui::End();
}

void BossEnemy::Bulletremove()
{
	normalBullets.remove_if([](std::unique_ptr<EnemyNormalBullet>& bullet)
		{
			return bullet->IsDead();
		});

}
