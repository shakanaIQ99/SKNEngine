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
	player->Wt->translation_.y = 2.0f;
}

void Player::Update()
{

	player->Wt->translation_.x += input->GetLStick(true, true).x * 0.5f;
	player->Wt->translation_.z += input->GetLStick(true, true).y * 0.5f;
	

	player->Update(camera);
}

void Player::Draw()
{
	player->Draw();
}
