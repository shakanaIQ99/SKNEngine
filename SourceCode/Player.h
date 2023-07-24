#pragma once
#include<ObjModel.h>
#include<OBJ3D.h>
#include<WorldTronsform.h>
#include<Camera.h>
#include<Input.h>
#include<Sprite2D.h>
#include<SpriteCommon.h>
#include<PlayerBullet.h>


class Player
{
public:
	void SetStruct(ObjModel* _model,OBJ3D* _obj,Camera* _camera,Input* _input,SpriteCommon* spcommon, TextureManager* texmana);

	void Init();

	void Update();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack(XMFLOAT3 flont);

	void Draw();

	void DrawUI();

	XMFLOAT3 GetPos()
	{
		return player->Wt->translation_;
	}

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };

private:
	Input* input;

	
	float move_speed;
	XMFLOAT3 moveVec;
	
	SpriteCommon* spCommon;
	TextureManager* texMana;
	Camera* camera;

	OBJ3D* player = nullptr;
	ObjModel* player_model = nullptr;

	std::unique_ptr<Sprite2D> sprite_Reticle;

	WorldTransform reticle;

	uint32_t reticleHandle;


	//弾関連
	std::list<std::unique_ptr<PlayerBullet>> bullets_;


	int bulletCT = 0;

	const int MAXBulletCT = 5;

	const int firelate = 15;

	int latetime;


	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	XMFLOAT3 VectorMat(XMFLOAT3 vector, XMMATRIX mat);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	XMFLOAT3 VectorMat(XMMATRIX mat, XMFLOAT3 vector);

};

const DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& v, float s);

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& v, float s);

const DirectX::XMFLOAT3 operator*(float s, const DirectX::XMFLOAT3& v);

const DirectX::XMFLOAT3 operator-= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);


