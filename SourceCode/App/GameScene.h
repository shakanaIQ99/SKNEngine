#pragma once
//ゲームシーン
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

enum struct SceneType
{
	TITLE,
	GAMESCENE
};

class GameScene
{

private:
	std::unique_ptr <SpriteCommon> spritecommon;

	TextureManager* texturemanager;


public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 当たり判定まとめ
	/// </summary>
	void ALLCol();

	/// <summary>
	/// 破棄
	/// </summary>
	void Finalize();


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
	void TitleDraw();

	/// <summary>
	/// メイン描画
	/// </summary>
	void GameDraw();

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
	std::unique_ptr<LightGroup> light;

	std::unique_ptr<Sprite2D> preTitle;
	uint32_t preTitleHandle;

	std::unique_ptr<Sprite2D> preTitle2;
	uint32_t preTitleHandle2;

	std::unique_ptr<Sprite2D> SceneCha;
	uint32_t SceneChaHandle;

	//std::unique_ptr<Object3D> Robo;
	//std::unique_ptr<Model> model;

	bool sceneChaflag = false;

	int SceneChangeTimer = 0;

	const int SceneChangeTime = 60;

	float tenmetu = 255.0f;

	float SceneAlpha = 255.0f;

	float decri = 3.0f;
	
	XMFLOAT3 flontVec = { 0,0,1.0f };
	float cameraDistance = 20.0f;

	SceneType scene = SceneType::TITLE;

};

