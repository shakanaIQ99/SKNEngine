#include"DxWindow.h"
#include"DirectXCommon.h"
#include"Object3D.h"
#include"Input.h"
#include<wrl.h>
#include"Sprite2D.h"
#include"SpriteCommon.h"
#include"WorldTronsform.h"
#include"TextureManager.h"
#include"ViewProjection.h"
#include"LightGroup.h"
#include<sstream>
#include<iomanip>
#include"ImGuiManager.h"
#include"imgui.h"
#include"CollisionPrimitive.h"
#include"Collision.h"
#include"FPS.h"



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxWindow* window = nullptr;

	window = DxWindow::GetInstance();
	window->CreateGameWindow();
#ifdef _DEBUG
		//デバッグレイヤーをオンに
		ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif	
	DirectXCommon* dxCommon = nullptr;

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(window);

	unique_ptr<FPS>fps;
	fps = std::make_unique<FPS>();
	fps->Initialize();

	ImGuiManager* imGuiManager = nullptr;
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(window->GetHwnd(),dxCommon);

	Input* input = nullptr;
	input = new Input();
	input->Initialize(window->GetHInstance(),window->GetHwnd());


	//HRESULT result;

	TextureManager* texturemanager = TextureManager::GetInstance();
	//TextureManager* texturemanager = nullptr;
	texturemanager->StaticInitialize(dxCommon);

	uint32_t blank = texturemanager->LoadTexture("Resources/white1x1.png");

	SpriteCommon* spritecommon = nullptr;
	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxCommon);
	
	Object3D::StaticInitialize(dxCommon->GetDevice(), DxWindow::window_width, DxWindow::window_height);
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	
	LightGroup* light = nullptr;
	light = LightGroup::Create();

	Object3D::SetLight(light);

	ViewProjection camera;
	camera.Initialize();

	
	XMFLOAT3 eye = { 0,0,-50 };

	int scenenum = 0;

	float ambientColor[3] = { 1,1,1 };

	float lightDir0[3] = { 0,0,-1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0.5f,1.0f,0 };
	float pointLightColor[3] = { 1,1,1 };

	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	light->SetDirLightActive(0, false);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);
	light->SetPointLightActive(0, true);

	
	Sphere sp;
	
	Ray ray;

	ray.dir = XMVectorSet(0, -1, 0, 0);



	bool Hit;
	bool rayHit;
	
	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	float a = 0.2f;

	Model* playerModel = Model::LoadFromOBJ("player");
	Model* enemyModel = Model::LoadFromOBJ("muso");
	Model* target = Model::LoadFromOBJ("arrow");
	Model* YUKA = Model::LoadFromOBJ("cube");


	WorldTransform playerWt;
	WorldTransform enemyWt;
	WorldTransform targetWt;
	WorldTransform groundWt;

	Object3D* player;
	Object3D* enemy;
	Object3D* arrow;
	Object3D* ground;

	player = Object3D::Create(&playerWt);
	player->SetModel(playerModel);
	enemy = Object3D::Create(&enemyWt);
	enemy->SetModel(enemyModel);
	arrow = Object3D::Create(&targetWt);
	arrow->SetModel(target);
	ground = Object3D::Create(&groundWt);
	ground->SetModel(YUKA);

	camera.SetEye(eye);
	
	enemyWt.scale_ = { 0.7f,0.7f,0.7f };
	playerWt.scale_ = { 0.7f,0.7f,0.7f };
	groundWt.scale_ = { 200.0f,1.0f,200.0f };
	groundWt.translation_.y = -10.0f;
	targetWt.scale_ = { 0.7f,0.7f,0.7f };
	targetWt.rotation_.x = 0.5f;
	
	XMFLOAT3 lightpos = { 0,50.0f,0 };

	light->SetPointLightPos(0, lightpos);

	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}

		input->InputUpdate();
		imGuiManager->Begin();
		static XMVECTOR lightDir = { 0,1,5,0 };
		
		switch (scenenum)
		{
		default:
			break;
		}






		player->Update(&camera);
		enemy ->Update(&camera);
		arrow ->Update(&camera);	
		ground->Update(&camera);
		light->Update();
		camera.Update();

		
		
		
		imGuiManager->End();
		dxCommon->PreDraw();

		Object3D::PreDraw(dxCommon->GetCommandList());
		
		
		player->Draw();
		
		enemy->Draw();
		
		arrow->Draw();
		
		ground->Draw();
		

		
		

		Object3D::PostDraw();

		spritecommon->PreDraw();
		
		
		spritecommon->PostDraw();

		imGuiManager->Draw();
		dxCommon->PostDraw();

		fps->Update();

	}
	
	delete input;
	delete playerModel;
	delete enemyModel;
	delete target;
	delete YUKA;
	delete player;
	delete enemy;
	delete arrow;
	delete ground;
	delete light;
	delete spritecommon;
	imGuiManager->Finalize();
	delete dxCommon;
	delete imGuiManager;
	texturemanager->DeleteInstance();
	//delete texturemanager;
	window->TerminateGameWindow();
}