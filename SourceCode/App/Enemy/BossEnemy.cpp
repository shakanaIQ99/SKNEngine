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
	ModelInit("muso");
	EnemyNormalBullet::SetModel(ObjModel::LoadFromOBJ("maru"));
	EnemyMine::SetModel(ObjModel::LoadFromOBJ("maru"));
	St->Wt.scale_ = { scale,scale,scale };
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
	//エフェクトや弾周りの初期化
	const std::list<std::unique_ptr<EnemyNormalBullet>>& Bullets = GetBullets();
	for (const std::unique_ptr<EnemyNormalBullet>& bullet : Bullets)
	{

		bullet->OnCollision();

	}
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
	BossAtk = AtkPattern::MISSILE;
	BossMove = MovePattern::NONE;
	chargeMoveAniTimer = 0;
	chargeCool = 0;
	mineCool = 0;
	TargetTimer = 0;
	MoveTimer = 0;
	stopTimer = 0;
	WaitTimer = 0;
	endFlag = false;
	DeathTimer = 0;

	rotaVec = { 0,0,1.0f };
	DpRate = 0;
	scale = 4.0f;
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

	//平面上の距離
	XMFLOAT3 plUnderPos = player->GetUnderPos() - St->Wt.translation_;
	Lange = length(plUnderPos);

	sprite_HPbar->Wt.scale_.x = (30.0f * HP / MaxHP);

	St->Wt.translation_.y -= 0.5f;

	//地面押し戻し
	if (St->Wt.translation_.y - (St->Wt.scale_.y * 1.5f) < 0.0f)
	{
		St->Wt.translation_.y = (St->Wt.scale_.y * 1.5f);
	}

	chargeCool--;
	mineCool--;

	//突進してないときの基本的な行動テーブル
	if (BossAtk != AtkPattern::CHARGE&&!flag&&!Death())
	{
		
		
		XMFLOAT3 Flont = { 0,0,1.0f };
		normalize(Flont);
		normalize(plUnderPos);
		

		float p_pos = atan2(plUnderPos.x, plUnderPos.z);
		float c_vec = atan2(Flont.x, Flont.z);

		St->Wt.rotation_.y = (p_pos + c_vec);

		//行動state
		switch (BossMove)
		{
		case MovePattern::NONE:

			//stopTimer = stopTime;
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

	if (!flag && !Death())
	{
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

	//弾更新
	for (std::unique_ptr<EnemyNormalBullet>& bullet : Normalbullets_)
	{
		bullet->Update();
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
	for (std::unique_ptr<EnemyMine>& mine : Mines_)
	{
		mine->Draw();
	}
	for (std::unique_ptr<DeathParticle>& dp : deathPaticles)
	{
		dp->Draw();
	}

	St->Draw();
	XMFLOAT3 Head = St->Wt.translation_;

	Head.y += St->Wt.scale_.y;
	//白線描画
	if (AimMode)
	{
		LeserPoint.Draw(Head, TargetPos);
	}
}

void BossEnemy::DrawUI()
{
	if (HP > 0)
	{
		sprite_HPbar->Draw();
	}
}

void BossEnemy::HitParticle(XMFLOAT3 vec)
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
		//突進攻撃範囲内
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
				chargeMoveAniTimer = chargeMoveAniTime;
				

				
			}
			else if(mineCool<0)
			{
				BossAtk = AtkPattern::MINE;
				TargetVec = player->GetPos() - St->Wt.translation_;
				TargetVec.y = 0;
				normalize(TargetVec);
				mineThrowTimer = 0;
				mineThrowDeg = 0;
				mineCool = mineCoolTime;

				

			}
			else if(mineCool>0&& chargeCool>0)
			{
				TargetTimer = TargetTime;
				BossAtk = AtkPattern::SIMPLESHOT;
				BurstTime = BurstNum * BurstRate;
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
			MoveTimer = LongMoveTime;
		}
		else
		{
			MoveTimer = MidMoveTime;
		}
	}

	//stopTimer--;

}

void BossEnemy::BackMove()
{
	XMFLOAT3 moveVec = St->Wt.translation_ - player->GetPos();
	moveVec.y = 0;

	normalize(moveVec);

	moveVec *= 0.4f;

	St->Wt.translation_ += moveVec;

	MoveTimer--;
	if (MoveTimer < 0) { BossMove = MovePattern::NONE; }
}

void BossEnemy::CloseMove()
{
	XMFLOAT3 moveVec = player->GetPos() - St->Wt.translation_;
	moveVec.y = 0;

	normalize(moveVec);

	moveVec *= 0.4f;

	St->Wt.translation_ += moveVec;


	MoveTimer--;
	if (MoveTimer < 0) { BossMove = MovePattern::NONE; }
}


void BossEnemy::SimpleShot()
{

	XMFLOAT3 BulletVec;
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
		XMFLOAT3 Head = St->Wt.translation_;

		Head.y += St->Wt.scale_.y;
		//Head.y = 1.5f;
		AimMode = false;
		if (CriticalAim)
		{
			BulletVec = LinePrediction2(Head, player->GetPos(), player->GetPredictionPoint(), 2.0f) - Head;
		}
		else
		{
			BulletVec = player->GetPos() - Head;
		}
		normalize(BulletVec);

		BulletVec *= 2.0f;
		
		

		//発射レート
		if (BurstTime % BurstRate == 0)
		{
			std::unique_ptr <EnemyNormalBullet> newBullet = std::make_unique<EnemyNormalBullet>();
			newBullet->Initlize(Head, St->Wt.rotation_, BulletVec);

			Normalbullets_.push_back(std::move(newBullet));

		}
		BurstTime--;
		//射撃時間終わったら
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

	St->Wt.translation_ += TargetVec * 1.5f;

	if (chargeLenge + 5.0f < length(chargeMoved))
	{
		BossAtk = AtkPattern::NONE;
	}


}

void BossEnemy::HardShot()
{
	//照準
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
		XMFLOAT3 BulletVec = player->GetPos() - Head;
		normalize(BulletVec);

		XMMATRIX matRot[3];

		for (auto& e : matRot)
		{
			e = XMMatrixIdentity();
		}

		// スケール、回転、平行移動行列の計算
		matRot[0] *= XMMatrixRotationY(XMConvertToRadians(5.0f));
		matRot[1] *= XMMatrixRotationY(XMConvertToRadians(-5.0f));
		matRot[2] *= XMMatrixRotationY(XMConvertToRadians(0));

		for (size_t i = 0; i < 3; i++)
		{
			XMFLOAT3 HardBullet = myMath::VectorMat(BulletVec, matRot[i]);
			normalize(HardBullet);
			HardBullet *= 5.0f;

			std::unique_ptr <EnemyNormalBullet> newBullet = std::make_unique<EnemyNormalBullet>();
			newBullet->Initlize(Head, St->Wt.rotation_, HardBullet);

			Normalbullets_.push_back(std::move(newBullet));

		}
		
		BossAtk = AtkPattern::NONE;

		
	}
}

void BossEnemy::MissileShot()
{




}

void BossEnemy::MineAttack()
{
	XMFLOAT3 throwVec = { 0,0,0 };

	

	if (mineThrowTimer == 0)
	{
		for (size_t i = 0; i < mineNum; i++)
		{

			size_t preMineNum = 60*i;

			float deg = XMConvertToRadians(static_cast<float>(preMineNum));

			throwVec.x = TargetVec.x * cosf(deg) - TargetVec.z * sinf(deg);

			throwVec.z = TargetVec.x * sinf(deg) + TargetVec.z * cosf(deg);

			normalize(throwVec);
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
	const char* AtkModes[] = { "NONE", "SIMPLESHOT", "CHARGE","HARDSHOT","MISSILE","MINE"};
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
			BossAtk = AtkPattern::HARDSHOT;
			break;
		case 4:
			BossAtk = AtkPattern::MISSILE;
			break;
		case 5:
			BossAtk = AtkPattern::MINE;
			break;
		}

	}
	ImGui::Text("BossAtkPattern::%s", AtkModes[static_cast<int>(BossAtk)]);

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
			BossMove = MovePattern::NONE;
			break;
		case 1:
			BossMove = MovePattern::BACK;
			break;
		case 2:
			BossMove = MovePattern::CLOSEMOVE;
			break;
		}

	}
	ImGui::Text("BossMoveMode::%s", MoveModes[static_cast<int>(BossMove)]);
	ImGui::NewLine();
	ImGui::Text("HP::%5.2f", HP);
	ImGui::DragFloat("HP", &HP, 0.2f);
	ImGui::Checkbox("CriticalAim", &CriticalAim);

	ImGui::End();
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

XMFLOAT3 BossEnemy::LinePrediction2(XMFLOAT3 shotPosition, XMFLOAT3 targetPosition, XMFLOAT3 targetPrePosition, float bulletSpeed)
{
	
	XMFLOAT3 v3_Mv = targetPosition - targetPrePosition;
	XMFLOAT3 v3_Pos = targetPosition - shotPosition;

	//normalize(v3_Mv);
	//normalize(v3_Pos);

	//float A = myMath::VectorLengthSq(v3_Mv) - bulletSpeed * bulletSpeed;
	//float B = myMath::VectorDot(v3_Pos, v3_Mv);
	//float C = myMath::VectorLengthSq(v3_Pos);

	////0割禁止
	//if (A == 0 && B == 0)return targetPosition;
	//if (A == 0)return targetPosition + v3_Mv * (-C / B / 2);

	////虚数解はどうせ当たらないので絶対値で無視した
	//float D = sqrtf(fabsf(B * B - A * C));
	//return targetPosition + v3_Mv * PlusMin((-B - D) / A, (-B + D) / A);



	//ピタゴラスの定理から２つのベクトルの長さが等しい場合の式を作り
	//二次方程式の解の公式を使って弾が当たる予測時間を計算する
	float A = (v3_Mv.x * v3_Mv.x + v3_Mv.y * v3_Mv.y + v3_Mv.z * v3_Mv.z) - bulletSpeed * bulletSpeed;
	float B = 2 * (v3_Pos.x * v3_Mv.x + v3_Pos.y * v3_Mv.y + v3_Pos.z * v3_Mv.z);
	float C = (v3_Pos.x * v3_Pos.x + v3_Pos.y * v3_Pos.y + v3_Pos.z * v3_Pos.z);

	//0割り禁止処理
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

	//弾が当たる時間のフレームを計算する
	float flame1, flame2;
	//二次方程式の解の公式の判別式で分類
	float D = B * B - 4 * A * C;
	if (D > 0)
	{
		float E = sqrtf(D);
		flame1 = (-B - E) / (2 * A);
		flame2 = (-B + E) / (2 * A);
		//解は2つなので正の数の最小値を使う
		flame1 = PlusMin(flame1, flame2);
	}
	else
	{
		//虚数解
		//当たらないので今の位置を狙う
		flame1 = 0;
	}

	//予想位置を返す
	return targetPosition + v3_Mv * flame1;
}

void BossEnemy::Bulletremove()
{
	Normalbullets_.remove_if([](std::unique_ptr<EnemyNormalBullet>& bullet)
		{
			return bullet->IsDead();
		});
	Mines_.remove_if([](std::unique_ptr<EnemyMine>& mine)
		{
			return mine->IsDead();
		});

}