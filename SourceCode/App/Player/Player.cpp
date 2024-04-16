#include "Player.h"
#include "ImGuiManager.h"
#include "Math/Easing.h"
#include"Field.h"
#include"myMath.h"
#include"AudioManager.h"


void Player::Init()
{

	ModelInit("Player",true);


	

	prePP.reset(OBJ3D::Create());
	prePP->SetModel(model.get());
	bulletModel.reset(ObjModel::LoadFromOBJ("maru"));
	
	colBox.reset(OBJ3D::Create());
	colBox->SetModel(model.get());


	colBox->Wt.scale_ = St->Wt.scale_;
	colBox->Wt.color = { 1.0f,1.0f,1.0f,1.0f };
	
	St->Wt.scale_ = { 1.0f,1.0f,1.0f };
	St->Wt.translation_.y = 50.0f;

	prePP->Wt.scale_ = St->Wt.scale_;
	prePP->Wt.color = { 0,1.0f,0,1.0f };

	hp = maxhp;

	keyUI = std::make_unique<Sprite2D>();
	keyUI->Initialize("KeyUI");

	sprite_HPbar = std::make_unique<Sprite2D>();
	sprite_HPbar->Initialize("HpBar");
	sprite_HPbar->Wt.translation_ = { 200.0f,680.0f,0.0f };
	sprite_HPbar->Wt.scale_.x = 10.0f;
	sprite_HPbar->Wt.color = { 0.0f,1.0f,0.0f,1.0f };

	sprite_CoverHPbar = std::make_unique<Sprite2D>();
	sprite_CoverHPbar->Initialize("HpBar");
	sprite_CoverHPbar->Wt.translation_ = { 200.0f,680.0f,0.0f };
	sprite_CoverHPbar->Wt.scale_.x = 10.0f;
	sprite_CoverHPbar->Wt.color = { 0.15f,0.15f,0.15f,1.0f };

	sprite_ENGauge = std::make_unique<Sprite2D>();
	sprite_ENGauge->Initialize("HpBar");
	sprite_ENGauge->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 22.5f)*20.0f ,0.0f };
	sprite_ENGauge->Wt.scale_.x = enGaugeSize * static_cast<float>(gaugeEN) / static_cast<float>(maxGaugeEN);
	sprite_ENGauge->Wt.scale_.y *= 0.5f;
	sprite_ENGauge->Wt.color = { 0.0f,0.15f,0.75f,1.0f };

	sprite_CoverENGaugebar = std::make_unique<Sprite2D>();
	sprite_CoverENGaugebar->Initialize("HpBar");
	sprite_CoverENGaugebar->Wt.translation_ = { DxWindow::window_width / 2.0f,(DxWindow::window_height / 22.5f) * 20.0f ,0.0f };
	sprite_CoverENGaugebar->Wt.scale_.x = enGaugeSize;
	sprite_CoverENGaugebar->Wt.scale_.y *= 0.5f;
	sprite_CoverENGaugebar->Wt.color = { 0.15f,0.15f,0.15f,1.0f };

	sprite_Reticle = std::make_unique<Sprite2D>();
	sprite_Reticle->Initialize("Reticle");

	move_speed = 0.4f;

	gaugeEN = maxGaugeEN;


	prePlayer = St->Wt;
}

void Player::Reset()
{
	hp = maxhp;
	St->Wt.translation_ = { 0,50.0f,0 };
	St->Wt.scale_ = { 1.0f,1.0f,1.0f };

	colBox->Wt = St->Wt;

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
	sceneCameraTimer = 0;
	endFlag = false;
	deathTimer = 0;

	rotaVec = { 0,0,1.0f };
	mae = { 0,0,0 };
	dpRate = 0;
	scale = 1.0f;
	hpBarShakeNum = 0;
	diff = 0;
	
}

void Player::Update()
{
	prePlayer.rotation_ = St->Wt.rotation_;
	sprite_HPbar->Wt.translation_.x = 200.0f-(8.0f * (maxhp-hp));
	sprite_HPbar->Wt.scale_.x = (10.0f * hp / maxhp);
	prePlayer.translation_ =Lerp(prePlayer.translation_, St->Wt.translation_, 0.3f);

	sprite_ENGauge->Wt.scale_.x = enGaugeSize * static_cast<float>(gaugeEN) / static_cast<float>(maxGaugeEN);
	if (hp > maxhp)
	{
		hp = maxhp;
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
		if (lateTime <= 0)
		{
			Attack(camera->GetForwardVec());
			AudioManager::Play("shot");

			lateTime = fireLate;
		}
	}

	lateTime--;


	for (std::unique_ptr<DeathParticle>& dp : deathPaticles)
	{
		dp->Update();
	}
	

#ifdef _DEBUG
	ImGuiSet();
#endif

	LockOn();

	St->Update(camera->GetView());
	prePlayer.UpdateMatrix(camera->GetView());
	prePP->Update(camera->GetView());
	colBox->Wt= St->Wt;
	colBox->Update(camera->GetView());
	sprite_HPbar->Update();
	sprite_ENGauge->Update();
	sprite_CoverENGaugebar->Update();
	sprite_CoverHPbar->Update();
}

void Player::Damege(float dmg)
{
	hp -= dmg * static_cast<float>(1 - muteki);
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
	knockVec.Normalize();
	knockSpeed = knockSpeedNum;

}

void Player::Attack(Vector3 flont)
{
	const float kBulletSpeed = 5.0f;
	
	Vector3 velocity = flont;

	if (locked)
	{
		velocity = boss->translation_ - GetPos();
	}

	velocity.Normalize();

	/*velocity = VectorMat(velocity,player->Wt->matWorld_);
	normalize(velocity);*/

	BulletManager::CreateNormalBullet(bulletModel.get(), St->Wt.translation_, velocity, 0.5f, kBulletSpeed, Tag::PLAYER);

	
}

void Player::Move()
{
	EN();

	

	rotaVec = mae;
	moveVec = { 0,0,0 };
	Vector3 Flont = camera->GetForwardVec();
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

	mae.Normalize();

	
	
	Jump(mae);

	if ((moveVec.x == 0 && moveVec.z == 0))
	{
		mae = rotaVec;
	}
	Dash(mae);

	if (Input::GetPadButtonDown(XINPUT_GAMEPAD_B))
	{
		boostMode = !boostMode;
		
	}


	if (!dashFlag&& (moveVec.x != 0 || moveVec.z != 0))
	{
		St->Wt.translation_ += mae * (move_speed + (move_speed * boostMode));
		/*playerPredictionPoint = St->Wt.translation_ + mae * (move_speed + (move_speed * BoostMode));
		playerPredictionPoint.y = 1.5f;*/
	}
	
	if ((moveVec.x == 0 && moveVec.z == 0))
	{
		boostMode = false;
	}

	//St->Wt.translation_.y -= 0.5f;

	if (St->Wt.translation_.y - (St->Wt.scale_.y * 1.5f) < 0.0f)
	{
		St->Wt.translation_.y = (St->Wt.scale_.y * 1.5f);
		onGround = true;
	}
	else
	{
		onGround = false;
	}



	if (St->Wt.translation_.x + St->Wt.scale_.x > Field::GetArea() || St->Wt.translation_.x - St->Wt.scale_.x < -Field::GetArea())
	{
		St->Wt.translation_.x = Field::GetArea() * (abs(St->Wt.translation_.x) / St->Wt.translation_.x) - St->Wt.scale_.x * (abs(St->Wt.translation_.x) / St->Wt.translation_.x);
	}
	if (St->Wt.translation_.z + St->Wt.scale_.z >= Field::GetArea() || St->Wt.translation_.z - St->Wt.scale_.z <= -Field::GetArea())
	{
		St->Wt.translation_.z = Field::GetArea() * (abs(St->Wt.translation_.z) / St->Wt.translation_.z) - St->Wt.scale_.z * (abs(St->Wt.translation_.z) / St->Wt.translation_.z);
	}
	
	//prePP->Wt.translation_ = prePlayer.translation_;

}

void Player::Jump(Vector3 front)
{
	if (!overHeat)
	{
		if (Input::GetPadButtonDown(XINPUT_GAMEPAD_A) && !dashFlag && onGround)
		{
			jumpFlag = true;
			jumpTimer = 0;
			jumpVec = front;
			useEN = true;
			gaugeEN -= jumpUseGauge;
			AudioManager::Play("jump");
		}
		if (Input::GetPadButton(XINPUT_GAMEPAD_A) && !dashFlag && !jumpFlag)
		{
			jumpTimer = 0;
			St->Wt.translation_.y += airUpSpead;
			gaugeEN -= airUseGauge;
		}
	}

	

	if (jumpFlag)
	{
		jumpTimer++;

		upSpeed = EaseInSine(upSpeadNum, 0.0f, static_cast<float>(jumpTimer), static_cast<float>(jumpTime));

		St->Wt.translation_ += jumpVec * upSpeed;

		if (jumpTimer >= jumpTime)
		{
			jumpFlag = false;
			jumpTimer = 0;
		}
	}
	else
	{
		if (!dashFlag)
		{
			jumpTimer++;
			upSpeed = EaseOutSine(0.0f, -upSpeadNum, static_cast<float>(jumpTimer), static_cast<float>(jumpTime));
		}
		if (jumpTimer >= jumpTime)
		{
			jumpTimer = jumpTime;
		}
	}

	St->Wt.translation_.y += upSpeed;

}

void Player::Dash(Vector3 front)
{
	if (Input::GetPadButtonDown(XINPUT_GAMEPAD_X) && !dashFlag&&!overHeat)
	{
		dashFlag = true;
		dashVec = front;
		dashTimer = 0;
		gaugeEN -= dashUseGauge;
		useEN = true;
		jumpTimer = 0;
		jumpFlag = false;
		upSpeed = 0;
		boostMode = true;
		AudioManager::Play("dash");
	}

	if (dashFlag)
	{
		dashTimer++;

		dashSpeed = EaseInQuint(dashSpeadNum, 0.0f, static_cast<float>(dashTimer), static_cast<float>(dashTime));

		St->Wt.translation_ += dashVec * dashSpeed;

		if (dashTimer >= dashTime)
		{
			dashFlag = false;
			boostMode = true;
		}
	}
}

void Player::EN()
{
	if (infEN)
	{
		gaugeEN = maxGaugeEN;
	}

	if (useEN)
	{
		regenENCoolTimer = regenENCoolTime;
		useEN = false;
	}

	if (gaugeEN <= 0&&!overHeat)
	{
		overHeat = true;
		overHeatENCoolTimer = overHeatENCoolTime;
	}

	if (overHeat)
	{
		overHeatENCoolTimer--;
		regenENCoolTimer = 0;
		gaugeEN += recoveryENGauge;
		if (overHeatENCoolTimer <= 0)
		{
			overHeat = false;
		}
	}
	

	if (regenENCoolTimer > 0)
	{
		regenENCoolTimer--;
	}
	else
	{
		gaugeEN+=regenEN;
	}

	if (gaugeEN >= maxGaugeEN)
	{
		gaugeEN = maxGaugeEN;
	}

	

}

void Player::LockOn()
{
	
	if (ScLock(boss))
	{

		Lock2DPos = WorldToMonitor(boss->translation_);
		locked = true;
	}
	else
	{
		Lock2DPos = { DxWindow::window_width / 2.0f,DxWindow::window_height / 2.0f };
		locked = false;
	}
}

bool Player::ScLock(WorldTransform* prewt)
{
	Matrix4 Pos = prewt->matWorld_;
	Pos *= camera->GetView()->GetMAtView();
	Pos *= camera->GetView()->GetMatProjection();

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
	dpRate++;
	deathTimer++;

	scale = EaseInSine(1.0f, 0, static_cast<float>(deathTimer), static_cast<float>(deathTime));

	St->Wt.scale_ = { scale,scale,scale };

	if (dpRate >= dpRateNum)
	{

		std::unique_ptr <DeathParticle> newBullet = std::make_unique<DeathParticle>();
		newBullet->CreateDeathParticle(St->Wt.translation_, St->Wt.rotation_, -rotaVec,scale/3.0f, { 0,0.3f,1.0f,1.0f });

		deathPaticles.push_back(std::move(newBullet));
		dpRate = 0;
	}
	if (deathTimer >= deathTime)
	{
		endFlag = true;
		deathTimer = deathTime;
	}
	
}

Vector2 Player::WorldToMonitor(Vector3 pos)
{
	Vector3 positionReticle = pos;

	Matrix4 matViewport = Matrix4::Viewport(0, 0, static_cast<float>(DxWindow::window_width), static_cast<float>(DxWindow::window_height), 0, 1.0f);

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = camera->GetView()->GetMAtView() * camera->GetView()->GetMatProjection() * matViewport;
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
	ImGui::Text("HP::%5.2f", hp);
	ImGui::DragFloat("HP", &hp, 0.2f);
	ImGui::DragFloat("HPposX", &sprite_HPbar->Wt.translation_.x, 0.5f);
	ImGui::DragFloat("HPposY", &sprite_HPbar->Wt.translation_.y, 0.5f);
	ImGui::DragFloat("HPSizeX", &sprite_HPbar->Wt.scale_.x, 0.5f);
	ImGui::DragFloat("HPSizeY", &sprite_HPbar->Wt.scale_.y, 0.5f);
	ImGui::NewLine();
	ImGui::Text("ENGauge::%d", gaugeEN);
	ImGui::Text("OverHeat::%d", overHeat);
	ImGui::NewLine();
	ImGui::Text("BoostMode::%d", boostMode);
	ImGui::Checkbox("InfEN", &infEN);
	ImGui::Checkbox("muteki", &muteki);
	ImGui::Checkbox("colLock", &colLock);


	ImGui::End();

}

void Player::Draw()
{
	for (std::unique_ptr<DeathParticle>& dp : deathPaticles)
	{
		dp->Draw();
	}

	colBox->Draw();
	if (colLock)
	{
		St->Draw();
	}
	//prePP->Draw();
}

void Player::DrawUI()
{
	if (overHeat)
	{
		sprite_ENGauge->Wt.color = { 1.0f,0,0,1.0f };
	}
	else
	{
		sprite_ENGauge->Wt.color = { 0.0f, 0.15f, 0.75f, 1.0f };
	}

	sprite_Reticle->Draw(Lock2DPos.x - 64.0f, Lock2DPos.y - 64.0f, Lock2DPos.x + 64.0f, Lock2DPos.y + 64.0f);
	sprite_CoverHPbar->Draw();
	sprite_CoverENGaugebar->Draw();
	keyUI->Draw(1280.0f-64.0f, 720.0f-128.0f, 1280.0f, 720.0f);
	if (gaugeEN > 0)
	{
		sprite_ENGauge->Draw();
	}
	if (hp > 0)
	{
		sprite_HPbar->Draw();
	}
	

	
}

void Player::TitleUpdate()
{



#ifdef _DEBUG
	ImGuiSet();
#endif
	St->Update(camera->GetView());
}

void Player::StartUpdate()
{
	sceneCameraTimer++;
	St->Wt.translation_.y = EaseOutQuint(50.0f, 1.5f, static_cast<float>(sceneCameraTimer), static_cast<float>(sceneCameraTime));

	if (sceneCameraTimer >= sceneCameraTime)
	{
		sceneCameraTimer = sceneCameraTime;
		startFlag = true;
	}
	prePlayer.translation_ = St->Wt.translation_;
#ifdef _DEBUG
	ImGuiSet();
#endif
	St->Update(camera->GetView());
}


