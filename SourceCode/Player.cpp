#include "Player.h"

void Player::SetStruct(ObjModel* _model, OBJ3D* _obj, ViewProjection* _camera, Input* _input)
{
	player = _obj;
	camera = _camera;
	player_model = _model;
	input = _input;

	player->SetModel(player_model);

}

void Player::Init()
{
	player->Wt->scale_ = { 1.0f,1.0f,1.0f };
}

void Player::Update()
{

	if (input->GetKey(DIK_RIGHT))
	{
		player->Wt->translation_.x += 0.5f;
	}
	if (input->GetKey(DIK_LEFT))
	{
		player->Wt->translation_.x -= 0.5f;
	}
	if (input->GetKey(DIK_UP))
	{
		player->Wt->translation_.z += 0.5f;
	}
	if (input->GetKey(DIK_DOWN))
	{
		player->Wt->translation_.z -= 0.5f;
	}

	
	

	player->Update(camera);
}

void Player::Draw()
{
	player->Draw();
}
