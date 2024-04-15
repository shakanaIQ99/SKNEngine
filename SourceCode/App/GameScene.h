#pragma once
#include"DirectXCommon.h"
#include"DxWindow.h"
#include"ViewProjection.h"
//#include"Object3D.h"
#include"ImGuiManager.h"
#include"BaseScene.h"
#include"SpriteCommon.h"
//#include"Model.h"
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
#include"Field.h"
#include"DeathParticle.h"
#include "BulletManager.h"
#include"AudioManager.h"

enum struct SceneType
{
	TITLE,
	GAMESCENE,
	CLEARSCENE,
	GAMEOVER
};

class GameScene:public BaseScene
{

public:

	void Initialize()override;

	void Update()override;

	void Draw()override;

	void ALLCol();

	void Finalize() override
	{
		delete light;
		delete skydome_model;
		delete skydome;
		AudioManager::Stop("bgm");
	}

private:

	/// <summary>
	/// メイン更新
	/// </summary>
	void GameUpdate();

	void StartUpdate();

	/// <summary>
	/// ImGui関係
	/// </summary>
	void ImGuiView();

	Player player;
	BossEnemy boss;
	Field field;

	float cameraX = 0;
	float cameraZ = 0;
	float rotateX = 0;
	float rotateY = 0;

	float cameraRotateX = 0;
	float cameraRotateY = 9.45f;

	float cameraDPI = 0.05f;


private:
	Camera camera;
	LightGroup* light = nullptr;

	ObjModel* skydome_model = nullptr;
	OBJ3D* skydome = nullptr;


	Vector3 flontVec = { 0,0,1.0f };
	float cameraDistance = 20.0f;


	

	bool sceneChaflag = false;
	bool endSceneChaflag = false;

	int sceneChangeTimer = 0;

	Vector3 sceneCamera = { 0,0,0 };

	int sceneCameraTimer = 0;

	const int sceneCameraTime = 120;

	int clearSceneTimer = 0;

	const int clearSceneTime = 60;

	bool hitcheck = false;



};

