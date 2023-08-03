#include "BossEnemy.h"
#include"ImGuiManager.h"
void BossEnemy::Init()
{
	ModelInit("chr_sword");
	EnemyNormalBullet::SetModel(ObjModel::LoadFromOBJ("maru"));
	transform.scale_ = { 4.0f,4.0f,4.0f };
	St->color = { 1.0f,0,0,1.0f };
	LeserPoint.Init();
}

void BossEnemy::Update()
{
	//•½–Êã‚Ì‹——£
	XMFLOAT3 plUnderPos = player->GetUnderPos() - transform.translation_;
	Lange = length(plUnderPos);

	switch (BossAtk)
	{
	case NONE:

		AimMode = false;
		AtkTable();

		break;
	case SIMPLESHOT:

		SimpleShot();

		break;
	case CHARGE:
		break;
	case LASER:
		break;
	}

	for (std::unique_ptr<EnemyNormalBullet>& bullet : Normalbullets_)
	{
		bullet->Update();
	}

	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x + 400, ImGui::GetMainViewport()->WorkPos.y + 10 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Boss", NULL, window_flags);

	ImGui::Text("Position");
	ImGui::DragFloat("X", &transform.translation_.x, 0.5f);
	ImGui::DragFloat("Y", &transform.translation_.y, 0.5f);
	ImGui::DragFloat("Z", &transform.translation_.z, 0.5f);
	ImGui::NewLine();
	ImGui::Text("Lange::%5.2f",Lange);
	ImGui::DragFloat("Min", &LangeMin, 0.5f);
	ImGui::DragFloat("Max", &LangeMax, 0.5f);
	ImGui::NewLine();
	static int AtkmodeNum = 0;
	const char* AtkModes[] = { "NONE", "SIMPLESHOT", "CHARGE","LASER"};
	ImGui::Combo("AtkmodeNumCombo", &AtkmodeNum, AtkModes, IM_ARRAYSIZE(AtkModes));
	ImGui::SameLine();
	if (ImGui::Button("Change"))
	{
		switch (AtkmodeNum)
		{
			case 0:
				BossAtk = NONE;
				break;
			case 1:
				BossAtk = SIMPLESHOT;
				break;
			case 2:
				BossAtk = CHARGE;
				break;
			case 3:
				BossAtk = LASER;
				break;
		}
		
	}
	ImGui::Text("BossMode::%s", AtkModes[BossAtk]);

	ImGui::End();

	St->Update(camera->getView());
}

void BossEnemy::Draw()
{
	for (std::unique_ptr<EnemyNormalBullet>& bullet : Normalbullets_)
	{
		bullet->Draw();
	}

	St->Draw();
	XMFLOAT3 Head = transform.translation_;

	Head.y += transform.scale_.y;
	if (AimMode)
	{
		LeserPoint.Draw(Head,TargetPos);
	}
}

void BossEnemy::DrawUI()
{
}

void BossEnemy::AtkTable()
{
	

	if (Lange < LangeMax)
	{
		TargetTimer = TargetTime;
		BossAtk = SIMPLESHOT;
		BurstTime= BurstNum * BurstRate;

	}


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
		XMFLOAT3 Head = transform.translation_;

		Head.y += transform.scale_.y;
		AimMode = false;
		XMFLOAT3 BulletVec = TargetPos - Head;
		normalize(BulletVec);

		BulletVec *= 2.0f;

		if (BurstTime % BurstRate == 0)
		{
			std::unique_ptr <EnemyNormalBullet> newBullet = std::make_unique<EnemyNormalBullet>();
			newBullet->Initlize(Head, transform.rotation_, BulletVec);

			Normalbullets_.push_back(std::move(newBullet));

		}
		BurstTime--;
		if (BurstTime <= 0)
		{
			BossAtk = NONE;

		}
	}

}

void BossEnemy::Bulletremove()
{
	Normalbullets_.remove_if([](std::unique_ptr<EnemyNormalBullet>& bullet)
		{
			return bullet->IsDead();
		});

}
