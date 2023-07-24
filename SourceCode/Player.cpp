#include "Player.h"

void Player::SetStruct(ObjModel* _model, OBJ3D* _obj, Camera* _camera, Input* _input, SpriteCommon* spcommon, TextureManager* texmana)
{
	player = _obj;
	camera = _camera;
	player_model = _model;
	input = _input;
	spCommon = spcommon;
	texMana = texmana;

	player->SetModel(player_model);

}

void Player::Init()
{
	
	reticleHandle = texMana->LoadTexture("Resources/Reticle.png");

	player->Wt->scale_ = { 1.0f,1.0f,1.0f };
	player->Wt->translation_.y = 10.0f;



	sprite_Reticle = std::make_unique<Sprite2D>();
	sprite_Reticle->Initialize(spCommon, &reticle, reticleHandle);
	reticle.translation_ = { DxWindow::window_width / 2.0f,DxWindow::window_height / 2.0f ,0.0f };
	move_speed = 0.4f;
}

void Player::Update()
{


	moveVec = { 0,0,0 };
	XMFLOAT3 Flont = camera->getForwardVec();
	Flont.y = 0;
	normalize(Flont);

	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});


	XMFLOAT2 inputnum = input->GetLStick(true, true);

	if (input->GetPadButtonDown(XINPUT_GAMEPAD_A))
	{
		player->Wt->translation_.y = 10.0f;
	}

	moveVec.x += (float)inputnum.x / SHRT_MAX;
	moveVec.z += (float)inputnum.y / SHRT_MAX;

	float p_pos = atan2(moveVec.x, moveVec.z);
	float c_vec = atan2(Flont.x, Flont.z);

	player->Wt->rotation_.y = (p_pos + c_vec);

	XMFLOAT3 mae = { 0,0,1.0f };

	mae = VectorMat(mae, player->Wt->matWorld_);

	normalize(mae);

	if (moveVec.x != 0 || moveVec.z != 0)
	{
		player->Wt->translation_ += mae * move_speed;
	}

	player->Wt->translation_.y -= 0.5f;
	
	if (player->Wt->translation_.y < 2.0f)
	{
		player->Wt->translation_.y = 2.0f;
	}

	if (input->GetRTriggerDown())
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

	player->Update(camera->getView());
	sprite_Reticle->Update();
}

void Player::Attack(XMFLOAT3 flont)
{
	const float kBulletSpeed = 5.0f;
	
	
	XMFLOAT3 velocity = flont;

	normalize(velocity);

	/*velocity = VectorMat(velocity,player->Wt->matWorld_);
	normalize(velocity);*/

	XMFLOAT3 BulletStart = player->Wt->translation_ ;
	velocity *= kBulletSpeed;


	std::unique_ptr <PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initlize(camera, BulletStart, player->Wt->rotation_, velocity);

	bullets_.push_back(std::move(newBullet));

	
}

void Player::Draw()
{
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}

	player->Draw();
}

void Player::DrawUI()
{
	sprite_Reticle->Draw();
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