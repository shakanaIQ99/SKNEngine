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
#include"ParticleManager.h"
#include"FbxLoader.h"





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
	ViewProjection camera;
	camera.Initialize();

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	Object3D::SetDevice(dxCommon->GetDevice());
	Object3D::SetCamera(&camera);
	Object3D::CreateGraphicsPipeline();

	ImGuiManager* imGuiManager = nullptr;
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(window->GetHwnd(), dxCommon);

	Input* input = nullptr;
	input = new Input();
	input->Initialize(window->GetHInstance(), window->GetHwnd());

	unique_ptr<FPS>fps;
	fps = std::make_unique<FPS>();
	fps->Initialize();

	//HRESULT result;

	TextureManager* texturemanager = TextureManager::GetInstance();
	//TextureManager* texturemanager = nullptr;
	texturemanager->StaticInitialize(dxCommon);

	uint32_t blank = texturemanager->LoadTexture("Resources/white1x1.png");

	SpriteCommon* spritecommon = nullptr;
	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxCommon);

	
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	uint32_t tex1 = texturemanager->LoadTexture("Resources/visual.png");
	uint32_t tex2 = texturemanager->LoadTexture("Resources/puragomi.jpg");
	uint32_t tex3 = texturemanager->LoadTexture("Resources/effect1.png");
	uint32_t tex4 = texturemanager->LoadTexture("Resources/effect3.png");

	
	
	Model* model1 = nullptr;
	Object3D* objec1 = nullptr;

	model1 = FbxLoader::GetInstance()->LoadModelFlomFile("cube");
	WorldTransform atm;
	atm.CreateConstBuffer(dxCommon->GetDevice());

	objec1 = new Object3D();
	objec1->Initilaize(&atm);
	objec1->SetModel(model1);
	
	WorldTransform wt3;
	WorldTransform wt4;
	WorldTransform p1;
	WorldTransform p2;

	Sprite2D* sprite = nullptr;
	sprite = new Sprite2D();
	sprite->Initialize(spritecommon, &wt3, tex1);
	Sprite2D* sprite2 = nullptr;
	sprite2 = new Sprite2D();
	sprite2->Initialize(spritecommon, &wt4, tex2);

	LightGroup* light = nullptr;
	light = LightGroup::Create();

	
	
	

	ParticleManager* particleMan = nullptr;
	ParticleManager* particleMan2 = nullptr;

	particleMan = ParticleManager::Create(tex3,&p1);
	particleMan2 = ParticleManager::Create(tex4,&p2);

	

	//
	sprite2->Wt->translation_.y = 5.0f;
	

	
	XMFLOAT3 eye = { 0,20,-200 };



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

	wt3.translation_ = { 100.0f,100.0f,0.0f };

	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	float a = 0.2f;

	wt3.scale_ = { 0.5f,0.5f,0 };
	wt4.scale_ = { 0.5f,0.5f,0 };

	for (int i = 0; i < 100; i++)
	{
		const float rnd_pos = 20.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		particleMan->Add(150, pos, vel, acc, 1.0f, 0.0f);

	}
	for (int j = 0; j < 100; j++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 1.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 1.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 1.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 4.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 4.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 4.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		particleMan2->Add(300, pos, vel, acc, 1.0f, 0.0f);

	}


	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}

		input->InputUpdate();
		imGuiManager->Begin();
		static XMVECTOR lightDir = { 0,1,5,0 };

	

		if (input->GetKey(DIK_RIGHT))
		{
			eye.x += 0.2f;
		}
		if (input->GetKey(DIK_LEFT))
		{
			eye.x -= 0.2f;
		}
		if (input->GetKey(DIK_UP))
		{
			eye.y += 0.2f;
		}
		if (input->GetKey(DIK_DOWN))
		{
			eye.y -= 0.2f;
		}
		
		
		particleMan->Update(&camera);
		particleMan2->Update(&camera);

		light->Update();

		
		sprite->Update();
		sprite2->Update();
		camera.SetEye(eye);
		camera.SetTarget({ 0,20,0 });
		camera.Update();
		objec1->Update();


	

		imGuiManager->End();
		dxCommon->PreDraw();

		objec1->Draw(dxCommon->GetCommandList());
		/*Object3D::PreDraw(dxCommon->GetCommandList());
		
		Object3D::PostDraw();*/
		ParticleManager::PreDraw(dxCommon->GetCommandList());

		// 3Dオブクジェクトの描画
		//particleMan->Draw();
		//particleMan2->Draw();


		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>

		// 3Dオブジェクト描画後処理
		ParticleManager::PostDraw();

		spritecommon->PreDraw();

		//sprite->Draw({0,0});
		//sprite2->DrawClip({ 80.0f,180.0f },{200.0f,100.0f},{});

		spritecommon->PostDraw();

		imGuiManager->Draw();
		dxCommon->PostDraw();

		fps->Update();

	}

	delete input;
	
	delete particleMan;
	delete particleMan2;
	delete objec1;
	delete model1;
	delete sprite;
	delete sprite2;
	delete light;
	delete spritecommon;
	FbxLoader::GetInstance()->Finalize();
	imGuiManager->Finalize();
	delete dxCommon;
	delete imGuiManager;
	texturemanager->DeleteInstance();
	//delete texturemanager;
	window->TerminateGameWindow();
}