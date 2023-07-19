#pragma once
#include <DirectXCommon.h>
#include <DxWindow.h>
#include"ViewProjection.h"
#include <Object3D.h>
#include <Input.h>
#include"ImGuiManager.h"
#include"imgui.h"
#include "TextureManager.h"
#include <SpriteCommon.h>
#include <Model.h>
#include <Sprite2D.h>
#include <LightGroup.h>
#include <ParticleManager.h>
#include"CollisionPrimitive.h"
#include"Collision.h"



class GameScene
{

private:
	Input* input = nullptr;
	ImGuiManager* imGuiManager = nullptr;
	SpriteCommon* spritecommon = nullptr;
	TextureManager* texturemanager;


public:

	void Init(DxWindow* dxwindow,DirectXCommon* dxcommon);

	void Update();

	void Draw(DirectXCommon* dxcommon);

	void Finalize();


private:
	ViewProjection camera;
	LightGroup* light = nullptr;
	

	uint32_t tex1;
	uint32_t tex2;
	uint32_t tex3;
	uint32_t tex4;

	Model* model1 = nullptr;
	Object3D* objec1 = nullptr;

	Sprite2D* sprite;
	

	WorldTransform atm;
	WorldTransform wt3;
	WorldTransform wt4;
	
	XMFLOAT3 eye;

};

