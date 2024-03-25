#include "Player.h"
#include "ImGuiManager.h"
#include "Math/Easing.h"
#include"Field.h"
#include"myMath.h"
#include"AudioManager.h"


void Player::Init()
{

	ModelInit("Player");


	

	prePP.reset(OBJ3D::Create());
	prePP->SetModel(model.get());
	bulletModel.reset(ObjModel::LoadFromOBJ("maru"));
	
	colBox.reset(OBJ3D::Create());
	colBox->SetModel(ObjModel::LoadFromOBJ("maru"));


	colBox->Wt.scale_ = St->Wt.scale_;
	colBox->Wt.color = { 1.0f,1.0f,1.0f,1.0f };
	
	
	
	reticleHandle = texMana->LoadTexture("Resources/Reticle.png");
	LockHandle = texMana->LoadTexture("Resources/Lock.png");
	HpBarHandle = texMana->LoadTexture("Resources/HpBar.png");
	St->Wt.scale_ = { 1.0f,1.0f,1.0f };
	St->Wt.translation_.y = 50.0f;

	prePP->Wt.scale_ = St->Wt.scale_;
	prePP->Wt.color = { 0,1.0f,0,1.0f };

	HP = MaxHP;

	KeyUI = std::make_unique<Sprite2D>();
	KeyUI->Initialize(spCommon, texMana->LoadTexture("Resources/preKeyUI.png"));

	sprite_HPbar = std::make_unique<Sprite2D>();
	sprite_HPbar->Initialize(spCommon, HpBarHandle);
	sprite_HPbar->Wt.translation_ = { 200.0f,680.0f,0.0f };
	sprite_HPbar->Wt.scale_.x = 10.0f;
	sprite_HPbar->Wt.color = { 0.0f,1.0f,0.0f,1.0f };

	sprite_CoverHPbar = std::make_unique<Sprite2D>();
	sprite_CoverHPbar->Initialize(spCommon,  HpBarHandle);
	sprite_CoverHPbar->Wt.translation_ = { 200.0f,680.0f,0.0f };
	sprite_CoverHPbar->Wt.scale_.x = 10.0f;
	sprite_CoverHPbar->Wt.color = { 0.15f,0.15f,0.15f,1.0f };

	sprite_ENGauge = std::make_unique<Sprite2D>();
	sprite_ENGauge->Initialize(spCommon, HpBarHandle);
	sprite_ENGauge->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 22.5f)*20.0f ,0.0f };
	sprite_ENGauge->Wt.scale_.x = ENGaugeSize * static_cast<float>(ENGauge) / static_cast<float>(ENMAXGauge);
	sprite_ENGauge->Wt.scale_.y *= 0.5f;
	sprite_ENGauge->Wt.color = { 0.0f,0.15f,0.75f,1.0f };

	sprite_CoverENGaugebar = std::make_unique<Sprite2D>();
	sprite_CoverENGaugebar->Initialize(spCommon, HpBarHandle);
	sprite_CoverENGaugebar->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 22.5f) * 20.0f ,0.0f };
	sprite_CoverENGaugebar->Wt.scale_.x = ENGaugeSize;
	sprite_CoverENGaugebar->Wt.scale_.y *= 0.5f;
	sprite_CoverENGaugebar->Wt.color = { 0.15f,0.15f,0.15f,1.0f };

	sprite_Reticle = std::make_unique<Sprite2D>();
	sprite_Reticle->Initialize(spCommon, reticleHandle);
	sprite_Reticle->Wt.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 2.0f ,0.0f };
	sprite_Reticle->Wt.scale_={ 1.0f,1.0f,1.0f };

	sprite_Lock= std::make_unique<Sprite2D>();
	sprite_Lock->Initialize(spCommon, LockHandle);

	move_speed = 0.4f;

	ENGauge = ENMAXGauge;


	prePlayer = St->Wt;
}

void Player::Reset()
{
	HP = MaxHP;
	St->Wt.translation_ = { 0,50.0f,0 };
	St->Wt.scale_ = { 1.0f,1.0f,1.0f };

	const std::list<std::unique_ptr<DeathParticle>>& Dps = GetDps();
	for (const std::unique_ptr<DeathParticle>& Dp : Dps)
	{

		Dp->Death();

	}
	
	deathPaticles.remove_if([](std::unique_ptr<DeathParticle>& dp)
		{
			return dp->IsDead();
		});

	startFlag = false;
	SceneCameraTimer = 0;
	endFlag = false;
	DeathTimer = 0;

	rotaVec = { 0,0,1.0f };
	mae = { 0,0,0 };
	DpRate = 0;
	scale = 1.0f;
	hpBarShakeNum = 0;
	diff = 0;
	
}

void Player::Update()
{
	prePlayer.rotation_ = St->Wt.rotation_;
	sprite_HPbar->Wt.translation_.x = 200.0f-(8.0f * (MaxHP-HP));
	sprite_HPbar->Wt.scale_.x = (10.0f * HP / MaxHP);
	prePlayer.translation_ =lerp(prePlayer.translation_, St->Wt.translation_, 0.3f);

	sprite_ENGauge->Wt.scale_.x = ENGaugeSize * static_cast<float>(ENGauge) / static_cast<float>(ENMAXGauge);
	if (HP > MaxHP)
	{
		HP = MaxHP;
	}

	deathPaticles.remove_if([](std::unique_ptr<DeathParticle>& dp)
		{
			return dp->IsDead();
		});

	if (!Death())
	{
		if (knockBack)
		{
			St->Wt.translation_ += knockVec * knockSpeed;
			knockSpeed -= 0.5f;
			if (knockSpeed <= 0)
			{
				knockBack = false;
			}
		}
		else
		{
			Move();
		}
		
	}
	else
	{
		DeathAnimetion();
	}
	
	
	if (Input::GetRTrigger()&& !Death())
	{
		if (latetime <= 0)
		{
			Attack(camera->getForwardVec());

			latetime = firelate;
		}
	}

	latetime--;


	for (std::unique_ptr<DeathParticle>& dp : deathPaticles)
	{
		dp->Update();
	}
	
	sprite_Lock->Wt.translation_ = { Lock2DPos.x,Lock2DPos.y,0.0f };

#ifdef _DEBUG
	ImGuiSet();
#endif

	LockOn();

	St->Update(camera->getView());
	prePlayer.UpdateMatrix(camera->getView());
	prePP->Update(camera->getView());
	sprite_Reticle->Update();
	colBox->Wt.translation_ = St->Wt.translation_;
	colBox->Wt.scale_ = St->Wt.scale_;
	colBox->Update(camera->getView());
	sprite_HPbar->Update();
	sprite_ENGauge->Update();
	sprite_CoverENGaugebar->Update();
	sprite_CoverHPbar->Update();
	sprite_Lock->Update();
}

void Player::Damege(float dmg)
{
	HP -= dmg * static_cast<float>(1 - Muteki);
	AudioManager::Play("hit");
}

void Player::HitParticle(Vector3 vec)
{
	if (!Death())
	{
		for (size_t i = 0; i < 4; i++)
		{
			std::unique_ptr <DeathParticle> newBullet = std::make_unique<DeathParticle>();
			newBullet->CreateHitParticle(St->Wt.translation_, St->Wt.rotation_, vec, 0.5f, { 0,0.3f,1.0f,1.0f });

			deathPaticles.push_back(std::move(newBullet));
		}
	}
	
}

void Player::KnockBack(Vector3 vec)
{
	knockBack = true;
	knockVec = vec;
	knockVec.y = 0;
	knockVec.normalize();
	knockSpeed = knockSpeedNum;

}

void Player::Attack(Vector3 flont)
{
	const float kBulletSpeed = 5.0f;
	
	Vector3 velocity = flont;

	if (Locked)
	{
		velocity = boss->translation_ - GetPos();
	}

	velocity.normalize();

	/*velocity = VectorMat(velocity,player->Wt->matWorld_);
	normalize(velocity);*/

	BulletManager::CreateNormalBullet(bulletModel.get(), St->Wt.translation_, velocity, 0.5f, kBulletSpeed, Tag::PLAYER);

	
}

void Player::Move()
{
	EN();

	

	rotaVec = mae;
	moveVec = { 0,0,0 };
	Vector3 Flont = camera->getForwardVec();
	Flont.y = 0;

	playerPredictionPoint = St->Wt.translation_;
	
	
	Vector2 inputnum = Input::GetLStick(true, true);

	moveVec.x += (float)inputnum.x / SHRT_MAX;
	moveVec.z += (float)inputnum.y / SHRT_MAX;


	float p_pos = atan2(moveVec.x, moveVec.z);
	float c_vec = atan2(Flont.x, Flont.z);




	mae = { 0,0,0 };

	if ((moveVec.x != 0 || moveVec.z != 0))
	{
		mae = { 0,0,1.0f };
		diff = p_pos+c_vec;
		/*St->Wt.rotation_.y = (p_pos + c_vec);*/	
	}
	St->Wt.rotation_.y = myMath::LerpShortAngle(St->Wt.rotation_.y, diff, 0.5f);

	mae *= St->Wt.matWorld_;

	mae.normalize();

	
	
	Jump(mae);

	if ((moveVec.x == 0 && moveVec.z == 0))
	{
		mae = rotaVec;
	}
	Dash(mae);

	if (Input::GetPadButtonDown(XINPUT_GAMEPAD_B))
	{
		BoostMode = !BoostMode;
	}


	if (!DashFlag&& (moveVec.x != 0 || moveVec.z != 0))
	{
		St->Wt.translation_ += mae * (move_speed + (move_speed * BoostMode));
		/*playerPredictionPoint = St->Wt.translation_ + mae * (move_speed + (move_speed * BoostMode));
		playerPredictionPoint.y = 1.5f;*/
	}
	
	if ((moveVec.x == 0 && moveVec.z == 0))
	{
		BoostMode = false;
	}

	//St->Wt.translation_.y -= 0.5f;

	if (St->Wt.translation_.y - (St->Wt.scale_.y * 1.5f) < 0.0f)
	{
		St->Wt.translation_.y = (St->Wt.scale_.y * 1.5f);
		OnGround = true;
	}
	else
	{
		OnGround = false;
	}



	if (St->Wt.translation_.x + St->Wt.scale_.x > Field::GetArea() || St->Wt.translation_.x - St->Wt.scale_.x < -Field::GetArea())
	{
		St->Wt.translation_.x = Field::GetArea() * (abs(St->Wt.translation_.x) / St->Wt.translation_.x) - St->Wt.scale_.x * (abs(St->Wt.translation_.x) / St->Wt.translation_.x);
	}
	if (St->Wt.translation_.z + St->Wt.scale_.z >= Field::GetArea() || St->Wt.translation_.z - St->Wt.scale_.z <= -Field::GetArea())
	{
		St->Wt.translation_.z = Field::GetArea() * (abs(St->Wt.translation_.z) / St->Wt.translation_.z) - St->Wt.scale_.z * (abs(St->Wt.translation_.z) / St->Wt.translation_.z);
	}
	
	prePP->Wt.translation_ = prePlayer.translation_;

}

void Player::Jump(Vector3 front)
{
	if (!OverHeat)
	{
		if (Input::GetPadButtonDown(XINPUT_GAMEPAD_A) && !DashFlag && OnGround)
		{
			JumpFlag = true;
			JumpTimer = 0;
			JumpVec = front;
			UseEN = true;
			ENGauge -= JumpUseGauge;
			AudioManager::Play("aaaa");
		}
		if (Input::GetPadButton(XINPUT_GAMEPAD_A) && !DashFlag && !JumpFlag)
		{
			JumpTimer = 0;
			St->Wt.translation_.y += AirUpSpead;
			ENGauge -= AirUseGauge;
		}
	}

	

	if (JumpFlag)
	{
		JumpTimer++;

		Upspeed = easeInSine(UpSpeadNum, 0.0f, static_cast<float>(JumpTimer), static_cast<float>(JumpTime));

		St->Wt.translation_ += JumpVec * Upspeed;

		if (JumpTimer >= JumpTime)
		{
			JumpFlag = false;
			JumpTimer = 0;
		}
	}
	else
	{
		if (!DashFlag)
		{
			JumpTimer++;
			Upspeed = easeOutSine(0.0f, -UpSpeadNum, static_cast<float>(JumpTimer), static_cast<float>(JumpTime));
		}
		if (JumpTimer >= JumpTime)
		{
			JumpTimer = JumpTime;
		}
	}

	St->Wt.translation_.y += Upspeed;

}

void Player::Dash(Vector3 front)
{
	if (Input::GetPadButtonDown(XINPUT_GAMEPAD_X) && !DashFlag&&!OverHeat)
	{
		DashFlag = true;
		DashVec = front;
		DashTimer = 0;
		ENGauge -= DashUseGauge;
		UseEN = true;
		JumpTimer = 0;
		JumpFlag = false;
		Upspeed = 0;
		BoostMode = true;
	}

	if (DashFlag)
	{
		DashTimer++;

		dashspeed = easeInQuint(DashSpeadNum, 0.0f, static_cast<float>(DashTimer), static_cast<float>(DashTime));

		St->Wt.translation_ += DashVec * dashspeed;

		if (DashTimer >= DashTime)
		{
			DashFlag = false;
			BoostMode = true;
		}
	}
}

void Player::EN()
{
	if (InfEN)
	{
		ENGauge = ENMAXGauge;
	}

	if (UseEN)
	{
		RegenENCoolTimer = RegenENCoolTime;
		UseEN = false;
	}

	if (ENGauge <= 0&&!OverHeat)
	{
		OverHeat = true;
		OverHeatENCoolTimer = OverHeatENCoolTime;
	}

	if (OverHeat)
	{
		OverHeatENCoolTimer--;
		RegenENCoolTimer = 0;
		ENGauge += RecoveryENGauge;
		if (OverHeatENCoolTimer <= 0)
		{
			OverHeat = false;
		}
	}
	

	if (RegenENCoolTimer > 0)
	{
		RegenENCoolTimer--;
	}
	else
	{
		ENGauge+=RegenEN;
	}

	if (ENGauge >= ENMAXGauge)
	{
		ENGauge = ENMAXGauge;
	}

	

}

void Player::LockOn()
{
	
	if (ScLock(boss))
	{

		Lock2DPos = WorldToMonitor(boss->translation_);
		sprite_Reticle->Wt.translation_ = { Lock2DPos.x,Lock2DPos.y ,0.0f };
		Locked = true;
	}
	else
	{
		Lock2DPos = { DxWindow::window_width / 2.0f,DxWindow::window_height / 2.0f };
		sprite_Reticle->Wt.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 2.0f ,0.0f };
		Locked = false;
	}
}

bool Player::ScLock(WorldTransform* prewt)
{
	Matrix4 Pos = prewt->matWorld_;
	Pos *= camera->getView()->GetMAtView();
	Pos *= camera->getView()->GetMatProjection();

	float objZ = Pos.GetTranslation().z;

	Vector2 scr_pos = WorldToMonitor(prewt->translation_);

	if (0 < scr_pos.x && (DxWindow::window_width)  > scr_pos.x && 
		0 < scr_pos.y &&(DxWindow::window_height)> scr_pos.y && objZ > 0)
	{
		return true;
	}

	else
	{
		return false;
	}
}

void Player::DeathAnimetion()
{
	DpRate++;
	DeathTimer++;

	scale = easeInSine(1.0f, 0, static_cast<float>(DeathTimer), static_cast<float>(DeathTime));

	St->Wt.scale_ = { scale,scale,scale };

	if (DpRate >= DpRateNum)
	{

		std::unique_ptr <DeathParticle> newBullet = std::make_unique<DeathParticle>();
		newBullet->CreateDeathParticle(St->Wt.translation_, St->Wt.rotation_, -rotaVec,scale/3.0f, { 0,0.3f,1.0f,1.0f });

		deathPaticles.push_back(std::move(newBullet));
		DpRate = 0;
	}
	if (DeathTimer >= DeathTime)
	{
		endFlag = true;
		DeathTimer = DeathTime;
	}
	
}

Vector2 Player::WorldToMonitor(Vector3 pos)
{
	Vector3 positionReticle = pos;

	Matrix4 matViewport = Matrix4::Viewport(0, 0, static_cast<float>(DxWindow::window_width), static_cast<float>(DxWindow::window_height), 0, 1.0f);

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = camera->getView()->GetMAtView() * camera->getView()->GetMatProjection() * matViewport;
	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Matrix4::ProjectionDivW(positionReticle,matViewProjectionViewport);

	//スプライトのレティクルに座標設定
	return Vector2(positionReticle.x, positionReticle.y);
}

void Player::ImGuiSet()
{
	//ImguI
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->WorkPos.x + 800, ImGui::GetMainViewport()->WorkPos.y + 10 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 500 });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Player", NULL, window_flags);

	ImGui::Text("Position");
	ImGui::DragFloat("X", &St->Wt.translation_.x, 0.5f);
	ImGui::DragFloat("Y", &prePP->Wt.translation_.y, 0.5f);
	ImGui::DragFloat("Z", &St->Wt.translation_.z, 0.5f);
	ImGui::NewLine();
	ImGui::Text("HP::%5.2f", HP);
	ImGui::DragFloat("HP", &HP, 0.2f);
	ImGui::DragFloat("HPposX", &sprite_HPbar->Wt.translation_.x, 0.5f);
	ImGui::DragFloat("HPposY", &sprite_HPbar->Wt.translation_.y, 0.5f);
	ImGui::DragFloat("HPSizeX", &sprite_HPbar->Wt.scale_.x, 0.5f);
	ImGui::DragFloat("HPSizeY", &sprite_HPbar->Wt.scale_.y, 0.5f);
	ImGui::NewLine();
	ImGui::Text("ENGauge::%d", ENGauge);
	ImGui::Text("OverHeat::%d", OverHeat);
	ImGui::NewLine();
	ImGui::Text("BoostMode::%d", BoostMode);
	ImGui::Checkbox("InfEN", &InfEN);
	ImGui::Checkbox("Muteki", &Muteki);
	ImGui::Checkbox("colLock", &colLock);


	ImGui::End();

}

void Player::Draw()
{
	for (std::unique_ptr<DeathParticle>& dp : deathPaticles)
	{
		dp->Draw();
	}

	St->Draw();
	if (colLock)
	{
		colBox->Draw();
	}
	//prePP->Draw();
}

void Player::DrawUI()
{
	if (OverHeat)
	{
		sprite_ENGauge->Wt.color = { 1.0f,0,0,1.0f };
	}
	else
	{
		sprite_ENGauge->Wt.color = { 0.0f, 0.15f, 0.75f, 1.0f };
	}

	sprite_Reticle->Draw();
	sprite_Lock->Draw();
	sprite_CoverHPbar->Draw();
	sprite_CoverENGaugebar->Draw();
	KeyUI->Draw(1280.0f-64.0f, 720.0f-128.0f, 1280.0f, 720.0f);
	if (ENGauge > 0)
	{
		sprite_ENGauge->Draw();
	}
	if (HP > 0)
	{
		sprite_HPbar->Draw();
	}
	

	
}

void Player::TitleUpdate()
{

	St->Wt.rotation_.y += 0.05f;

#ifdef _DEBUG
	ImGuiSet();
#endif
	St->Update(camera->getView());
}

void Player::StartUpdate()
{
	SceneCameraTimer++;
	St->Wt.translation_.y = easeOutQuint(50.0f, 1.5f, static_cast<float>(SceneCameraTimer), static_cast<float>(SceneCameraTime));

	if (SceneCameraTimer >= SceneCameraTime)
	{
		SceneCameraTimer = SceneCameraTime;
		startFlag = true;
	}
	prePlayer.translation_ = St->Wt.translation_;
#ifdef _DEBUG
	ImGuiSet();
#endif
	St->Update(camera->getView());
}


