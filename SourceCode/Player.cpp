#include "Player.h"
#include "ImGuiManager.h"


void Player::Init()
{

	ModelInit("Player");
	PlayerBullet::SetModel(ObjModel::LoadFromOBJ("maru"));
	
	
	reticleHandle = texMana->LoadTexture("Resources/Reticle.png");
	HpBarHandle = texMana->LoadTexture("Resources/HpBar.png");
	transform.scale_ = { 1.0f,1.0f,1.0f };
	transform.translation_.y = 10.0f;

	HP = MaxHP;

	sprite_HPbar = std::make_unique<Sprite2D>();
	sprite_HPbar->Initialize(spCommon, &HpBar, HpBarHandle);
	HpBar.translation_ = { 200.0f,680.0f,0.0f };
	HpBar.scale_.x = 10.0f;
	HpBar.color = { 0.0f,1.0f,0.0f,1.0f };

	sprite_CoverHPbar = std::make_unique<Sprite2D>();
	sprite_CoverHPbar->Initialize(spCommon, &CoverHpBar, HpBarHandle);
	CoverHpBar.translation_ = { 200.0f,680.0f,0.0f };
	CoverHpBar.scale_.x = 10.0f;
	CoverHpBar.color = { 0.15f,0.15f,0.15f,1.0f };

	sprite_Reticle = std::make_unique<Sprite2D>();
	sprite_Reticle->Initialize(spCommon, &reticle, reticleHandle);
	reticle.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 2.0f ,0.0f };
	reticle.scale_={ 0.5f,0.5f,0.5f };
	move_speed = 0.4f;
}

void Player::Update()
{

	HpBar.translation_.x = 200.0f-(8.0f * (MaxHP-HP));
	HpBar.scale_.x = (10.0f * HP / MaxHP);
	if (HP > MaxHP)
	{
		HP = MaxHP;
	}

	moveVec = { 0,0,0 };
	XMFLOAT3 Flont = camera->getForwardVec();
	Flont.y = 0;
	normalize(Flont);

	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	XMFLOAT2 inputnum = Input::GetLStick(true, true);

	moveVec.x += (float)inputnum.x / SHRT_MAX;
	moveVec.z += (float)inputnum.y / SHRT_MAX;

	float p_pos = atan2(moveVec.x, moveVec.z);
	float c_vec = atan2(Flont.x, Flont.z);

	transform.rotation_.y = (p_pos + c_vec);

	XMFLOAT3 mae = { 0,0,1.0f };

	mae = VectorMat(mae,transform.matWorld_);

	normalize(mae);

	if (moveVec.x != 0 || moveVec.z != 0)
	{
		transform.translation_ += mae * move_speed;
	}

	transform.translation_.y -= 0.5f;
	
	if (transform.translation_.y - (transform.scale_.y*1.5f)< 0.0f)
	{
		transform.translation_.y = (transform.scale_.y * 1.5f);
	}

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
	

	ImGuiSet();



	St->Update(camera->getView());
	sprite_Reticle->Update();
	sprite_HPbar->Update();
	sprite_CoverHPbar->Update();
}

void Player::Damege(float dmg)
{
	HP -= dmg;
}

void Player::Attack(XMFLOAT3 flont)
{
	const float kBulletSpeed = 5.0f;
	
	
	XMFLOAT3 velocity = flont;

	normalize(velocity);

	/*velocity = VectorMat(velocity,player->Wt->matWorld_);
	normalize(velocity);*/

	XMFLOAT3 BulletStart =transform.translation_ ;
	velocity *= kBulletSpeed;


	std::unique_ptr <PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initlize(BulletStart, transform.rotation_, velocity);

	bullets_.push_back(std::move(newBullet));

	
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
	ImGui::DragFloat("X", &transform.translation_.x, 0.5f);
	ImGui::DragFloat("Y", &transform.translation_.y, 0.5f);
	ImGui::DragFloat("Z", &transform.translation_.z, 0.5f);
	ImGui::NewLine();
	ImGui::Text("HP::%5.2f", HP);
	ImGui::DragFloat("HP", &HP, 0.2f);
	ImGui::DragFloat("HPposX", &HpBar.translation_.x, 0.5f);
	ImGui::DragFloat("HPposY", &HpBar.translation_.y, 0.5f);
	ImGui::DragFloat("HPSizeX", &HpBar.scale_.x, 0.5f);
	ImGui::DragFloat("HPSizeY", &HpBar.scale_.y, 0.5f);



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
	sprite_Reticle->Draw();
	sprite_CoverHPbar->Draw();
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