#include "BossEnemy.h"
#include"ImGuiManager.h"
#include"Easing.h"
#include"Field.h"
#include"myMath.h"
#include <random>
#include<cmath>

#include <iostream>     // cout
#include <ctime>        // time
#include <cstdlib>      // srand,rand

BossEnemy::BossEnemy()
{
}

void BossEnemy::Init()
{
	ModelInit("enemy");
	bulletModel.reset(ObjModel::LoadFromOBJ("maru"));
	//hbulletModel.reset(ObjModel::LoadFromOBJ("bit"));
	EnemyMine::SetModel(ObjModel::LoadFromOBJ("maru"));
	
	St->color = { 1.0f,1.0f,1.0f,1.0f };
	LeserPoint.Init();
	HP = MaxHP;

	colBox.reset(OBJ3D::Create());
	colBox->SetModel(ObjModel::LoadFromOBJ("maru"));


	HpBarHandle = texMana->LoadTexture("Resources/HpBar.png");


	colBox->Wt.scale_ = St->Wt.scale_;
	colBox->color = { 1.0f,1.0f,1.0f,1.0f };


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
	//エフェクトや弾周りの初期化

	const std::list<std::unique_ptr<EnemyMine>>& Mines = GetMines();
	for (const std::unique_ptr<EnemyMine>& mine : Mines)
	{

		mine->Destoroy();

	}
	//弾消し関数
	Bulletremove();
	//死亡時パーティクル初期化
	const std::list<std::unique_ptr<DeathParticle>>& Dps = GetDps();
	for (const std::unique_ptr<DeathParticle>& Dp : Dps)
	{

		Dp->Death();

	}

	deathPaticles.remove_if([](std::unique_ptr<DeathParticle>& dp)
		{
			return dp->IsDead();
		});

	//各パラメータ初期化
	//パターン
	BossAtk = AtkPattern::NONE;
	BossMove = MovePattern::NONE;
	//突撃攻撃関係
	chargeMoveAniTimer = 0;
	chargeCool = 0;
	//地雷関係
	mineCool = 0;

	//射撃関係
	TargetTimer = 0;
	aimingTargetPos = { 0,0,0 };
	criAimTimer = 0;
	//行動時間関係
	moveTimer = 0;
	stopTimer = 0;
	WaitTimer = 0;
	endFlag = false;
	stepMoveTimer = 0;
	//死亡時間
	DeathTimer = 0;

	prePos = { 0,0,0 };
	rotaVec = { 0,0,1.0f };
	preVec = { 0,0,0 };
	DpRate = 0;
	scale = 10.0f;
	crossLine = 0;
	St->Wt.scale_ = { scale,scale,scale };
}

void BossEnemy::Update(bool flag)
{
	//弾やパーティクルを消していく
	Bulletremove();
	deathPaticles.remove_if([](std::unique_ptr<DeathParticle>& dp)
		{
			return dp->IsDead();
		});

	//死んだとき
	if (Death())
	{
		DeathAnimetion();
	}

	/*preVec = { 0,0,0 };
	prePos = { 0,0,0 };*/

	//平面上の距離
	Vector3 plUnderPos = player->GetUnderPos() - St->Wt.translation_;
	Lange = plUnderPos.length();

	sprite_HPbar->Wt.scale_.x = (30.0f * HP / MaxHP);

	St->Wt.translation_.y -= 0.5f;

	//地面押し戻し
	if (St->Wt.translation_.y - (St->Wt.scale_.y/3.0f) < 0.0f)
	{
		St->Wt.translation_.y = (St->Wt.scale_.y / 3.0f);
	}

	chargeCool--;
	mineCool--;

	//突進してないときの基本的な行動テーブル
	if (BossAtk != AtkPattern::CHARGE&&!flag&&!Death())
	{
		
		
		Vector3 Flont = { 0,0,1.0f };
		Flont.normalize();
		plUnderPos.normalize();
		

		float p_pos = atan2(plUnderPos.x, plUnderPos.z);
		float c_vec = atan2(Flont.x, Flont.z);

		St->Wt.rotation_.y = (p_pos + c_vec);

		//行動state
		switch (BossMove)
		{
		case MovePattern::NONE:
			MoveTable();
			break;
		case MovePattern::BACK:
			BackMove();
			break;
		case MovePattern::CLOSEMOVE:
			CloseMove();
			break;
		case MovePattern::FANSHAPE:
			FanShapeMove();
			break;
		case MovePattern::SIDESTEP:
			SideStepMove();
			break;
		}
	}

	if (!flag && !Death())
	{
		aiming();
		//攻撃state
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
		case AtkPattern::HARDSHOT:
			HardShot();
			break;
		case AtkPattern::MISSILE:

			break;
		case AtkPattern::MINE:
			MineAttack();
			break;
		}
	}


	for (std::unique_ptr<EnemyMine>& mine : Mines_)
	{
		mine->Update();
	}


	//パーティクル更新
	for (std::unique_ptr<DeathParticle>& dp : deathPaticles)
	{
		dp->Update();
	}


#ifdef _DEBUG
	ImGuiSet();
#endif

	//エリア外行かないように
	if (St->Wt.translation_.x + St->Wt.scale_.x > Field::GetArea() || St->Wt.translation_.x - St->Wt.scale_.x < -Field::GetArea())
	{
		St->Wt.translation_.x = Field::GetArea() - St->Wt.scale_.x * (abs(St->Wt.translation_.x) / St->Wt.translation_.x);
	}
	if (St->Wt.translation_.z + St->Wt.scale_.z >= Field::GetArea() || St->Wt.translation_.z - St->Wt.scale_.z <= -Field::GetArea())
	{
		St->Wt.translation_.z = Field::GetArea() - St->Wt.scale_.z * (abs(St->Wt.translation_.z) / St->Wt.translation_.z);
	}

	colBox->Wt = St->Wt;

	St->Update(camera->getView());
	colBox->Update(camera->getView());
	sprite_HPbar->Update();
}

void BossEnemy::Damege(float dmg)
{
	HP -= dmg;
}

void BossEnemy::Draw()
{

	for (std::unique_ptr<EnemyMine>& mine : Mines_)
	{
		mine->Draw();
	}
	for (std::unique_ptr<DeathParticle>& dp : deathPaticles)
	{
		dp->Draw();
	}

	St->Draw();
	if (colLock)
	{

		colBox->Draw();
	}
	
	//白線描画
	if (AimMode)
	{
		LeserPoint.Draw(St->Wt.translation_, TargetPos);
	}
}

void BossEnemy::DrawUI()
{
	if (HP > 0)
	{
		sprite_HPbar->Draw();
	}
}

void BossEnemy::HitParticle(Vector3 vec)
{
	for (size_t i = 0; i < 10; i++)
	{
		std::unique_ptr <DeathParticle> newBullet = std::make_unique<DeathParticle>();
		newBullet->CreateHitParticle(St->Wt.translation_, St->Wt.rotation_, vec, 2.0f, { 1.f,0.0f,0.0f,1.0f });

		deathPaticles.push_back(std::move(newBullet));
	}
}

void BossEnemy::AtkTable()
{
	//攻撃間のクールタイム参照
	if (WaitTimer < 0)
	{
		//突進攻撃のレンジにいないとき
		if (Lange > LangeMax)
		{
			mt19937 mt{ random_device{}() };

			uniform_int_distribution<int> dist(1, 6);

			int aktmode = dist(mt);

			if (aktmode >=2)
			{
				TargetTimer = TargetTime;
				BossAtk = AtkPattern::SIMPLESHOT;
				BurstTime = BurstNum * BurstRate;
			}
			else
			{
				TargetTimer = TargetTime;
				BossAtk = AtkPattern::HARDSHOT;
				BurstTime = BurstNum * BurstRate;
			}


			

		}

		if (Lange > LangeLong)
		{
			mt19937 mt{ random_device{}() };

			uniform_int_distribution<int> dist(1, 6);

			int aktmode = dist(mt);

			if (aktmode >= 4)
			{
				TargetTimer = TargetTime;
				BossAtk = AtkPattern::SIMPLESHOT;
				BurstTime = BurstNum * BurstRate;
			}
			else
			{
				TargetTimer = TargetTime;
				BossAtk = AtkPattern::HARDSHOT;
				BurstTime = BurstNum * BurstRate;
			}




		}
		//突進攻撃範囲内
		if (Lange < LangeMax)
		{
			if (chargeCool < 0)
			{
				BossAtk = AtkPattern::CHARGE;
				ChargeAtkReset();
			}
			else if(mineCool<0)
			{
				BossAtk = AtkPattern::MINE;
				MineAttackReset();
			}
			else if(mineCool>0&& chargeCool>0)
			{
				BossAtk = AtkPattern::SIMPLESHOT;
				SimpleShotReset();
			}


		}

	}
	//攻撃間のクールタイムカウントダウン
	WaitTimer--;
}

void BossEnemy::MoveTable()
{
	if (stopTimer < 0)
	{
		bool TimeRand = rand() % 1;
		if (Lange > LangeMax)
		{
			BossMove = MovePattern::CLOSEMOVE;
			
		}
		else
		{
			BossMove = MovePattern::BACK;
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
	Vector3 moveVec = St->Wt.translation_ - player->GetPos();
	moveVec.y = 0;

	moveVec.normalize();

	moveVec *= 0.4f;

	St->Wt.translation_ += moveVec;

	moveTimer--;
	if (moveTimer < 0) { BossMove = MovePattern::NONE; }
}

void BossEnemy::BackMoveReset()
{
}

void BossEnemy::CloseMove()
{
	Vector3 moveVec = player->GetPos() - St->Wt.translation_;
	moveVec.y = 0;

	moveVec.normalize();

	moveVec *= 0.4f;

	St->Wt.translation_ += moveVec;


	moveTimer--;
	if (moveTimer < 0) { BossMove = MovePattern::NONE; }
}

void BossEnemy::CloseMoveReset()
{
}

void BossEnemy::SideStepMove()
{
	
	Vector3 stepToPos = prePos + (Vector3(verticalVec.x, 0, verticalVec.y) * static_cast<float>(rightVec - leftVec));

	St->Wt.translation_ = easeOutQuint(prePos, stepToPos, static_cast<float>(stepMoveTimer), static_cast<float>(stepMoveTime));

	if (++stepMoveTimer > stepMoveTime|| !leftVec && !rightVec)
	{
		BossMove = MovePattern::NONE;
		
	}


}

void BossEnemy::SideStepMoveReset()
{
	preVec = player->GetPos() - St->Wt.translation_;
	prePos = St->Wt.translation_;
	stepMoveTimer = 0;
	rightVec = false;
	leftVec = false;

	verticalVec = Vector2(preVec.GetXZ()).GetVerticalR();
	verticalVec.normalize();
	verticalVec *= stepDistance;

	if (!Field::OutOfArea(verticalVec + (St->Wt.translation_).GetXZ(), scale))
	{
		rightVec = true;
	}
	if (!Field::OutOfArea(-verticalVec + (St->Wt.translation_).GetXZ(), scale))
	{
		//leftVec = true;
	}

	if (leftVec && rightVec)
	{

	}




}


void BossEnemy::FanShapeMove()
{
}

void BossEnemy::FanShapeMoveReset()
{
}

void BossEnemy::SimpleShot()
{

	Vector3 BulletVec;
	//照準時間
	if (TargetTimer > 0)
	{
		if (TargetTimer > 10)
		{
			TargetPos = player->GetPos();

		}
		TargetTimer--;
		AimMode = true;
	}
	else//照準終わり時の攻撃
	{
		

		AimMode = false;
		if (CriticalAim)
		{
			BulletVec = aimingTargetPos - St->Wt.translation_;
		}
		else
		{
			BulletVec = player->GetPos() - St->Wt.translation_;
		}
		BulletVec.normalize();
		
		

		//発射レート
		if (BurstTime % BurstRate == 0)
		{

			BulletManager::CreateNormalBullet(bulletModel.get(), St->Wt.translation_, BulletVec, 0.5f, nBulletSpeed, Tag::ENEMYNORMAL);

		}
		BurstTime--;
		//射撃時間終わったら
		if (BurstTime <= 0)
		{
			BossAtk = AtkPattern::NONE;

		}
	}

}

void BossEnemy::SimpleShotReset()
{
	TargetTimer = TargetTime;
	BurstTime = BurstNum * BurstRate;
}

void BossEnemy::ChargeAtk()
{


	Vector3 chargeMoved = St->Wt.translation_ - prePos;
	chargeMoved.y = 0;

	St->Wt.translation_ += TargetVec * 1.5f;

	if (chargeLenge + 5.0f < chargeMoved.length())
	{
		BossAtk = AtkPattern::NONE;
	}


}

void BossEnemy::ChargeAtkReset()
{
	prePos = St->Wt.translation_;
	prePos.y = 0;
	TargetVec = player->GetPos() - St->Wt.translation_;
	TargetVec.y = 0;
	chargeLenge = TargetVec.length();
	TargetVec.normalize();
	chargeCool = chargeCoolTime;
	chargeMoveAniTimer = chargeMoveAniTime;
}

void BossEnemy::HardShot()
{
	//照準
	if (TargetTimer > 0)
	{
		if (TargetTimer > 10)
		{
			TargetPos = aimingTargetPos;

		}
		TargetTimer--;
		AimMode = true;
	}
	else
	{
		
		AimMode = false;
		Vector3 BulletVec = player->GetPos() - St->Wt.translation_;
		BulletVec.normalize();

		Matrix4 matRot[3];

		

		// スケール、回転、平行移動行列の計算
		matRot[0] *= Matrix4::RotationY(10.0f);
		matRot[1] *= Matrix4::RotationY(-10.0f);;
		matRot[2] *= Matrix4::RotationY(0);

		for (size_t i = 0; i < 3; i++)
		{
			Vector3 HardBullet = BulletVec * matRot[i];
			HardBullet.normalize();
			
			BulletManager::CreateHomingBullet(bulletModel.get(), St->Wt.translation_, HardBullet, &player->St->Wt.translation_, 1.5f, hBulletSpeed, Tag::ENEMYHARD);

		}
		
		BossAtk = AtkPattern::NONE;

		
	}
}

void BossEnemy::HardShotReset()
{
}

void BossEnemy::MissileShot()
{




}

void BossEnemy::MineAttack()
{
	Vector3 throwVec = { 0,0,0 };

	

	if (mineThrowTimer == 0)
	{
		for (size_t i = 0; i < mineNum; i++)
		{

			size_t preMineNum = 60*i;

			float deg = XMConvertToRadians(static_cast<float>(preMineNum));

			throwVec.x = TargetVec.x * cosf(deg) - TargetVec.z * sinf(deg);

			throwVec.z = TargetVec.x * sinf(deg) + TargetVec.z * cosf(deg);

			throwVec.normalize();
			throwVec *= 1.0f;

			std::unique_ptr <EnemyMine> newMine = std::make_unique<EnemyMine>();
			newMine->Initlize(St->Wt.translation_, St->Wt.rotation_, throwVec);

			Mines_.push_back(std::move(newMine));

		}
	}

	mineThrowTimer++;
	
	mineThrowDeg = easeOutQuint(0.0, 360.0f, static_cast<float>(mineThrowTimer), static_cast<float>(mineThrowTime));

	St->Wt.rotation_.y += XMConvertToRadians(mineThrowDeg);

	
	

	if (mineThrowTimer >= mineThrowTime)
	{
		
		mineThrowTimer = mineThrowTime;
		mineThrowDeg = 0;
		BossAtk = AtkPattern::NONE;
	}


}

void BossEnemy::MineAttackReset()
{
	TargetVec = player->GetPos() - St->Wt.translation_;
	TargetVec.y = 0;
	TargetVec.normalize();
	mineThrowTimer = 0;
	mineThrowDeg = 0;
	mineCool = mineCoolTime;
}

void BossEnemy::ImGuiSet()
{
	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x + 400, ImGui::GetMainViewport()->WorkPos.y + 10 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Boss", NULL, window_flags);
	ImGui::Text("Scale_X::%5.2f", St->Wt.scale_.x);
	ImGui::Text("Scale_Y::%5.2f", St->Wt.scale_.y);
	ImGui::Text("Scale_Z::%5.2f", St->Wt.scale_.z);
	ImGui::NewLine();
	ImGui::Text("colScale_X::%5.2f", colBox->Wt.scale_.x);
	ImGui::Text("colScale_Y::%5.2f", colBox->Wt.scale_.y);
	ImGui::Text("colScale_Z::%5.2f", colBox->Wt.scale_.z);

	ImGui::NewLine();
	ImGui::Text("Position");
	ImGui::DragFloat("X", &St->Wt.translation_.x, 0.5f);
	ImGui::DragFloat("Y", &St->Wt.translation_.y, 0.5f);
	ImGui::DragFloat("Z", &St->Wt.translation_.z, 0.5f);
	ImGui::NewLine();
	ImGui::Text("Lange::%5.2f", Lange);
	ImGui::DragFloat("Min", &LangeMin, 0.5f);
	ImGui::DragFloat("Max", &LangeMax, 0.5f);
	ImGui::NewLine();
	
	static int MovemodeNum = 0;
	const char* MoveModes[] = { "NONE", "BACK", "CLOSEMOVE","FANSHAPE","SIDESTEP" };
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
			BackMoveReset();
			break;
		case 2:
			BossMove = MovePattern::CLOSEMOVE;
			CloseMoveReset();
			break;
		case 3:
			BossMove = MovePattern::FANSHAPE;
			FanShapeMoveReset();
			break;
		case 4:
			BossMove = MovePattern::SIDESTEP;
			SideStepMoveReset();
			break;
		}

	}
	ImGui::Text("BossMoveMode::%s", MoveModes[static_cast<int>(BossMove)]);
	//ImGui::NewLine();
	//static int AtkmodeNum = 0;
	//const char* AtkModes[] = { "NONE", "SIMPLESHOT", "CHARGE","HARDSHOT","MISSILE","MINE" };
	//ImGui::Combo("##AtkmodeNumCombo", &AtkmodeNum, AtkModes, IM_ARRAYSIZE(AtkModes));
	//ImGui::SameLine();
	//if (ImGui::Button("Change"))
	//{
	//	switch (AtkmodeNum)
	//	{
	//	case 0:
	//		BossAtk = AtkPattern::NONE;
	//		break;
	//	case 1:
	//		TargetTimer = TargetTime;
	//		BossAtk = AtkPattern::SIMPLESHOT;
	//		BurstTime = BurstNum * BurstRate;
	//		break;
	//	case 2:
	//		BossAtk = AtkPattern::CHARGE;
	//		break;
	//	case 3:
	//		BossAtk = AtkPattern::HARDSHOT;
	//		break;
	//	case 4:
	//		BossAtk = AtkPattern::MISSILE;
	//		break;
	//	case 5:
	//		BossAtk = AtkPattern::MINE;
	//		break;
	//	}

	//}
	//ImGui::Text("BossAtkPattern::%s", AtkModes[static_cast<int>(BossAtk)]);

	const char* items[] = { "NONE", "SIMPLESHOT", "CHARGE","HARDSHOT","MISSILE","MINE" };
	static const char* current_item = items[0];
	static int* aaaaas = 0;
	if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(items[n],is_selected, ImGuiSelectableFlags_DontClosePopups))
			{
				current_item = items[n];
				BossAtk= static_cast<AtkPattern>(n);
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}

			
					
		}
		ImGui::EndCombo();
	}
	/*if (ImGui::Button("Change"))
	{
		switch (aaaaas)
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
			BossAtk = AtkPattern::HARDSHOT;
			break;
		case 4:
			BossAtk = AtkPattern::MISSILE;
			break;
		case 5:
			BossAtk = AtkPattern::MINE;
			break;
		}

	}*/
	ImGui::Text("aaa::%s", current_item);

	ImGui::NewLine();
	ImGui::Text("HP::%5.2f", HP);
	ImGui::DragFloat("HP", &HP, 0.2f);
	ImGui::Checkbox("CriticalAim", &CriticalAim);
	ImGui::Checkbox("colLock", &colLock);

	ImGui::End();
}

void BossEnemy::aiming()
{
	Vector3 criticalAimPos = LinePrediction2(St->Wt.translation_, player->GetPos(), player->GetPredictionPoint(), nBulletSpeed);

	float judgeLine = crossLine;

	aimingTargetPos = player->GetPos();

	aimingTargetPos = lerp(player->GetPos(), criticalAimPos, static_cast<float>(criAimTimer / criAimTime));

	criAimTimer++;
	/*LinePrediction2(St->Wt.translation_, player->GetPos(), player->GetPredictionPoint(), nBulletSpeed)*/
	if (criAimTimer > criAimTime)criAimTimer = criAimTime;

	Vector2 a = (player->GetPos() - St->Wt.translation_).GetXZ();
	Vector2 b = (criticalAimPos - St->Wt.translation_).GetXZ();

	a.normalize();
	b.normalize();

	crossLine = a.cross(b);
	if (myMath::sign(crossLine) != myMath::sign(judgeLine))criAimTimer = 0;

}

void BossEnemy::DeathAnimetion()
{

	DpRate++;
	DeathTimer++;

	scale = easeInSine(4.0f, 0, static_cast<float>(DeathTimer), static_cast<float>(DeathTime));

	St->Wt.scale_ = { scale,scale,scale };

	if (DpRate >= DpRateNum)
	{

		std::unique_ptr <DeathParticle> newBullet = std::make_unique<DeathParticle>();
		newBullet->CreateDeathParticle(St->Wt.translation_, St->Wt.rotation_, -rotaVec,scale/3.0f, { 1.0f,0.0f,0.0f,1.0f });

		deathPaticles.push_back(std::move(newBullet));
		DpRate = 0;
	}
	if (DeathTimer >= DeathTime)
	{
		endFlag = true;
		DeathTimer = DeathTime;
	}

	
}

Vector3 BossEnemy::LinePrediction2(Vector3 shotPosition, Vector3 targetPosition, Vector3 targetPrePosition, float bulletSpeed)
{
	
	Vector3 v3_Mv = targetPosition - targetPrePosition;
	Vector3 v3_Pos = targetPosition - shotPosition;

	
	float A = (v3_Mv.x * v3_Mv.x + v3_Mv.y * v3_Mv.y + v3_Mv.z * v3_Mv.z) - bulletSpeed * bulletSpeed;
	float B = 2 * (v3_Pos.x * v3_Mv.x + v3_Pos.y * v3_Mv.y + v3_Pos.z * v3_Mv.z);
	float C = (v3_Pos.x * v3_Pos.x + v3_Pos.y * v3_Pos.y + v3_Pos.z * v3_Pos.z);

	
	if (A == 0)
	{
		if (B == 0)
		{
			return targetPosition;
		}
		else
		{
			return targetPosition + v3_Mv * (-C / B);
		}
	}

	
	float flame1, flame2;
	float D = B * B - 4 * A * C;
	if (D > 0)
	{
		float E = sqrtf(D);
		flame1 = (-B - E) / (2 * A);
		flame2 = (-B + E) / (2 * A);
		
		flame1 = PlusMin(flame1, flame2);
	}
	else
	{
		
		flame1 = 0;
	}

	
	return targetPosition + v3_Mv * flame1;
}

void BossEnemy::Bulletremove()
{

	Mines_.remove_if([](std::unique_ptr<EnemyMine>& mine)
		{
			return mine->IsDead();
		});

}