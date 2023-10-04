#include "Player.h"
#include "ImGuiManager.h"
#include "Math/Easing.h"
#include"Field.h"

XMFLOAT3 VectorMatDivW(XMMATRIX mat, XMFLOAT3 pos)
{
	float w = pos.x * mat.r[0].m128_f32[3] + pos.y * mat.r[1].m128_f32[3] + pos.z * mat.r[2].m128_f32[3] + mat.r[3].m128_f32[3];

	XMFLOAT3 result =
	{
		(pos.x * mat.r[0].m128_f32[0] + pos.y * mat.r[1].m128_f32[0] + pos.z * mat.r[2].m128_f32[0] + mat.r[3].m128_f32[0]) / w,
		(pos.x * mat.r[0].m128_f32[1] + pos.y * mat.r[1].m128_f32[1] + pos.z * mat.r[2].m128_f32[1] + mat.r[3].m128_f32[1]) / w,
		(pos.x * mat.r[0].m128_f32[2] + pos.y * mat.r[1].m128_f32[2] + pos.z * mat.r[2].m128_f32[2] + mat.r[3].m128_f32[2]) / w
	};

	return result;
}

void Player::Init()
{

	ModelInit("Player");
	PlayerBullet::SetModel(ObjModel::LoadFromOBJ("maru"));
	
	
	reticleHandle = texMana->LoadTexture("Resources/Reticle.png");
	LockHandle = texMana->LoadTexture("Resources/Lock.png");
	HpBarHandle = texMana->LoadTexture("Resources/HpBar.png");
	St->Wt.scale_ = { 1.0f,1.0f,1.0f };
	St->Wt.translation_.y = 10.0f;

	HP = MaxHP;

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
}

void Player::Reset()
{
	HP = MaxHP;
	St->Wt.translation_ = { 0,10.0f,0 };
	const std::list<std::unique_ptr<PlayerBullet>>& Bullets = GetBullets();
	for (const std::unique_ptr<PlayerBullet>& p_bullet : Bullets)
	{
		
		p_bullet->OnCollision();
		
	}
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	

}

void Player::Update()
{

	sprite_HPbar->Wt.translation_.x = 200.0f-(8.0f * (MaxHP-HP));
	sprite_HPbar->Wt.scale_.x = (10.0f * HP / MaxHP);

	sprite_ENGauge->Wt.scale_.x = ENGaugeSize * static_cast<float>(ENGauge) / static_cast<float>(ENMAXGauge);
	if (HP > MaxHP)
	{
		HP = MaxHP;
	}

	

	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	Move();
	
	if (Input::GetRTrigger())
	{
		if (latetime <= 0)
		{
			Attack(camera->getForwardVec());

			latetime = firelate;
		}
	}

	latetime--;

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
	
	sprite_Lock->Wt.translation_ = { Lock2DPos.x,Lock2DPos.y,0.0f };

	ImGuiSet();

	LockOn();

	St->Update(camera->getView());
	sprite_Reticle->Update();
	sprite_HPbar->Update();
	sprite_ENGauge->Update();
	sprite_CoverENGaugebar->Update();
	sprite_CoverHPbar->Update();
	sprite_Lock->Update();
}

void Player::Damege(float dmg)
{
	HP -= dmg;
}

void Player::Attack(XMFLOAT3 flont)
{
	const float kBulletSpeed = 5.0f;
	
	XMFLOAT3 velocity = flont;

	if (Locked)
	{
		velocity = boss->translation_ - GetPos();
	}

	normalize(velocity);

	/*velocity = VectorMat(velocity,player->Wt->matWorld_);
	normalize(velocity);*/

	XMFLOAT3 BulletStart = St->Wt.translation_ ;
	velocity *= kBulletSpeed;


	std::unique_ptr <PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initlize(BulletStart, St->Wt.rotation_, velocity);

	bullets_.push_back(std::move(newBullet));

	
}

void Player::Move()
{
	EN();
	moveVec = { 0,0,0 };
	XMFLOAT3 Flont = camera->getForwardVec();
	Flont.y = 0;
	normalize(Flont);
	XMFLOAT2 inputnum = Input::GetLStick(true, true);

	moveVec.x += (float)inputnum.x / SHRT_MAX;
	moveVec.z += (float)inputnum.y / SHRT_MAX;

	float p_pos = atan2(moveVec.x, moveVec.z);
	float c_vec = atan2(Flont.x, Flont.z);

	St->Wt.rotation_.y = (p_pos + c_vec);

	XMFLOAT3 mae = { 0,0,0 };

	if ((moveVec.x != 0 || moveVec.z != 0))
	{
		mae = { 0,0,1.0f };
	}

	mae = VectorMat(mae, St->Wt.matWorld_);

	normalize(mae);

	
	Jump(mae);
	Dash(mae);

	if (Input::GetPadButtonDown(XINPUT_GAMEPAD_B))
	{
		BoostMode = !BoostMode;
	}


	if (!DashFlag)
	{
		St->Wt.translation_ += mae * (move_speed+(move_speed*BoostMode));
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
		St->Wt.translation_.x = Field::GetArea() - St->Wt.scale_.x * (abs(St->Wt.translation_.x) / St->Wt.translation_.x);
	}
	if (St->Wt.translation_.z + St->Wt.scale_.z >= Field::GetArea() || St->Wt.translation_.z - St->Wt.scale_.z <= -Field::GetArea())
	{
		St->Wt.translation_.z = Field::GetArea() - St->Wt.scale_.z * (abs(St->Wt.translation_.z) / St->Wt.translation_.z);
	}

}

void Player::Jump(XMFLOAT3 front)
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

void Player::Dash(XMFLOAT3 front)
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
	XMMATRIX Pos = prewt->matWorld_;
	Pos *= camera->getView()->GetMAtView();
	Pos *= camera->getView()->GetMatProjection();

	float objZ = Pos.r[3].m128_f32[2];

	XMFLOAT2 scr_pos = WorldToMonitor(prewt->translation_);

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

XMFLOAT2 Player::WorldToMonitor(XMFLOAT3 pos)
{
	XMFLOAT3 positionReticle = pos;

	XMMATRIX matViewport = {
		1280 / 2,0,0,0,
		0,-720 / 2,0,0,
		0,0,1,0,
		1280 / 2 + 0,720 / 2 + 0,0,1
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	XMMATRIX matViewProjectionViewport = camera->getView()->GetMAtView();
	matViewProjectionViewport *= camera->getView()->GetMatProjection();
	matViewProjectionViewport *= matViewport;

	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = VectorMatDivW(matViewProjectionViewport, positionReticle);

	//スプライトのレティクルに座標設定
	return XMFLOAT2(positionReticle.x, positionReticle.y);
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
	ImGui::DragFloat("Y", &St->Wt.translation_.y, 0.5f);
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


	ImGui::End();

}

void Player::Draw()
{
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}

	St->Draw();
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
	if (ENGauge > 0)
	{
		sprite_ENGauge->Draw();
	}
	if (HP > 0)
	{
		sprite_HPbar->Draw();
	}
	

	
}

XMFLOAT3 Player::VectorMat(XMFLOAT3 vector, XMMATRIX mat)
{
	XMFLOAT3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.r[0].m128_f32[0] + vector.y * mat.r[1].m128_f32[0] + vector.z * mat.r[2].m128_f32[0] + 0.0f * mat.r[3].m128_f32[0];
	changeVector.y = vector.x * mat.r[0].m128_f32[1] + vector.y * mat.r[1].m128_f32[1] + vector.z * mat.r[2].m128_f32[1] + 0.0f * mat.r[3].m128_f32[1];
	changeVector.z = vector.x * mat.r[0].m128_f32[2] + vector.y * mat.r[1].m128_f32[2] + vector.z * mat.r[2].m128_f32[2] + 0.0f * mat.r[3].m128_f32[2];

	return changeVector;
}

XMFLOAT3 Player::VectorMat(XMMATRIX mat, XMFLOAT3 vector)
{
	XMFLOAT3 changeVector = { 0,0,0 };

	changeVector.x = mat.r[0].m128_f32[0] * vector.x + mat.r[0].m128_f32[1] * vector.y + mat.r[0].m128_f32[2] * vector.z + mat.r[0].m128_f32[3] * 0.0f;
	changeVector.y = mat.r[1].m128_f32[0] * vector.x + mat.r[1].m128_f32[1] * vector.y + mat.r[1].m128_f32[2] * vector.z + mat.r[1].m128_f32[3] * 0.0f;
	changeVector.z = mat.r[2].m128_f32[0] * vector.x + mat.r[2].m128_f32[1] * vector.y + mat.r[2].m128_f32[2] * vector.z + mat.r[2].m128_f32[3] * 0.0f;

	return changeVector;
}

const DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& v, float s)
{

	v.x *= s;
	v.y *= s;
	v.z *= s;

	return v;
}

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& v, float s)
{
	DirectX::XMFLOAT3 temp(v);
	return temp *= s;
}

const DirectX::XMFLOAT3 operator*(float s, const DirectX::XMFLOAT3& v)
{
	return v * s;
}

const DirectX::XMFLOAT3 operator-= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{

	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	DirectX::XMFLOAT3 temp(v1);
	return temp -= v2;
}