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
#include"Field.h"
#include"DeathParticle.h"

enum struct SceneType
{
	TITLE,
	GAMESCENE,
	CLEARSCENE,
	GAMEOVER
};

class GameScene
{

private:
	SpriteCommon* spritecommon = nullptr;
	TextureManager* texturemanager;


public:

	void Init(SKNEngine::DirectXCommon* dxcommon);

	void Update();

	void Draw(SKNEngine::DirectXCommon* dxcommon);

	void ALLCol();

	void Finalize()
	{
		delete spritecommon;
		texturemanager->DeleteInstance();
		delete light;

		delete skydome_model;
		delete skydome;
	}

	

private:

private:
	/// <summary>
	/// タイトル更新
	/// </summary>
	void TitleUpdate();

	/// <summary>
	/// メイン更新
	/// </summary>
	void GameUpdate();

	/// <summary>
	/// タイトル描画
	/// </summary>
	void TitleDraw(SKNEngine::DirectXCommon* dxcommon);

	/// <summary>
	/// メイン描画
	/// </summary>
	void GameDraw(SKNEngine::DirectXCommon* dxcommon);

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


	XMFLOAT3 flontVec = { 0,0,1.0f };
	float cameraDistance = 20.0f;

	std::unique_ptr<Sprite2D> preTitle;
	uint32_t preTitleHandle;

	std::unique_ptr<Sprite2D> preTitle2;
	uint32_t preTitleHandle2;

	std::unique_ptr<Sprite2D> SceneCha;
	uint32_t SceneChaHandle;

	std::unique_ptr<Sprite2D> clearSc;
	uint32_t clearScHandle;
	uint32_t GameOverScHandle;

	bool sceneChaflag = false;
	bool endSceneChaflag = false;

	int SceneChangeTimer = 0;

	const int SceneChangeTime = 60;

	float tenmetu = 255.0f;

	float SceneAlpha = 255.0f;

	float SceneAlpha2 = 255.0f;

	float decri = 3.0f;


	SceneType scene = SceneType::TITLE;

	XMFLOAT3 sceneCamera = { 0,0,0 };

	int SceneCameraTimer = 0;

	const int SceneCameraTime = 120;

	int clearSceneTimer = 0;

	const int clearSceneTime = 60;



};

