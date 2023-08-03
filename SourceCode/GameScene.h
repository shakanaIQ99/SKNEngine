#pragma once
#include"DirectXCommon.h"
#include"DxWindow.h"
#include"ViewProjection.h"
#include"Object3D.h"
#include"ImGuiManager.h"
#include"TextureManager.h"
#include"SpriteCommon.h"
#include"Model.h"
#include"Sprite2D.h"
#include"LightGroup.h"
#include"ParticleManager.h"
#include"CollisionPrimitive.h"
#include"Collision.h"
#include"OBJ3D.h"
#include"ObjModel.h"
#include"Camera.h"
#include"Player.h"
#include"BossEnemy.h"
#include"Draw3DLine.h"



class GameScene
{

private:
	SpriteCommon* spritecommon = nullptr;
	TextureManager* texturemanager;


public:

	void Init(DxWindow* dxwindow,DirectXCommon* dxcommon);

	void Update();

	void Draw(DirectXCommon* dxcommon);

	void Finalize()
	{
		delete spritecommon;
		texturemanager->DeleteInstance();
		delete light;

		delete skydome_model;
		delete skydome;
		delete field_model;
		delete field;
	}

private:

	Player player;
	BossEnemy boss;

	float rotateX = 0;

	float cameraX = 0;
	float cameraZ = 0;
	float rotateY = 0;

	float cameraRotateX = 0;
	float cameraRotateY = 9.45f;

	float cameraDPI = 0.05f;


private:
	Camera camera;
	LightGroup* light = nullptr;

	ObjModel* skydome_model = nullptr;
	OBJ3D* skydome = nullptr;

	ObjModel* field_model = nullptr;
	OBJ3D* field = nullptr;

	WorldTransform skydome_wt;
	WorldTransform field_wt;

	
	XMFLOAT3 flontVec = { 0,0,1.0f };
	float cameraDistance = 20.0f;

};

