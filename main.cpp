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
#include"Light.h"
#include<sstream>
#include<iomanip>
//#include"Sprite.h"




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


	//HRESULT result;

	Input* input = nullptr;
	input = new Input();
	input->Initialize(window->GetHInstance(),window->GetHwnd());

	TextureManager* texturemanager = TextureManager::GetInstance();
	//TextureManager* texturemanager = nullptr;
	texturemanager->StaticInitialize(dxCommon);

	SpriteCommon* spritecommon = nullptr;
	spritecommon = new SpriteCommon();
	spritecommon->Initialize(dxCommon);
	
	Object3D::StaticInitialize(dxCommon->GetDevice(), DxWindow::window_width, DxWindow::window_height);
	Light::StaticInitialize(dxCommon->GetDevice());
	Model* skydome = Model::LoadFromOBJ("skydome");
	Model* model2 = Model::LoadFromOBJ("maru",true);
	Model* model = Model::LoadFromOBJ("chr_sword");
	Model* gra = Model::LoadFromOBJ("ground");
	WorldTransform ground;
	WorldTransform skywt;
	WorldTransform human;
	WorldTransform ball;
	WorldTransform wt3;
	WorldTransform wt4;
	uint32_t tex1 = texturemanager->LoadTexture("Resources/visual.png");
	uint32_t tex2 = texturemanager->LoadTexture("Resources/puragomi.jpg");

	Sprite2D* sprite = nullptr;
	sprite = new Sprite2D();
	sprite->Initialize(spritecommon,&wt3,tex1);
	Sprite2D* sprite2 = nullptr;
	sprite2 = new Sprite2D();
	sprite2->Initialize(spritecommon, &wt4,tex2);

	Light* light = nullptr;
	light = Light::Create();
	light->SetLightColor({ 1,1,1 });

	Object3D::SetLight(light);

	Object3D* obj1 = nullptr;
	Object3D* obj2 = nullptr;
	Object3D* objskydome = nullptr;
	Object3D* objground = nullptr;

	skywt.scale_ = { 0.5f,0.5f,0.5f };

	objskydome = Object3D::Create(&skywt);
	objskydome->SetModel(skydome);

	objground = Object3D::Create(&ground);
	objground->SetModel(gra);

	obj1 = Object3D::Create(&human);
	obj1->SetModel(model);
	obj2 = Object3D::Create(&ball);
	obj2->SetModel(model2);

	//
	sprite2->Wt->translation_.y = 5.0f;
	ViewProjection camera;
	camera.Initialize();

	XMFLOAT3 eye = { 0,0,-50 };

	ground.translation_.y = -10.0f;

	human.translation_.x = -10.0f;
	human.translation_.y = -5.0f;
	ball.translation_.x = 10.0f;

	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}

		input->InputUpdate();

		static XMVECTOR lightDir = { 0,1,5,0 };

		if (input->GetKey(DIK_W))
		{
			lightDir.m128_f32[1] += 1.0f;
		}
		if (input->GetKey(DIK_S))
		{
			lightDir.m128_f32[1] -= 1.0f;
		}
		if (input->GetKey(DIK_D))
		{
			lightDir.m128_f32[0] += 1.0f;
		}
		if (input->GetKey(DIK_A))
		{
			lightDir.m128_f32[0] -= 1.0f;
		}

		light->SetLightDir(lightDir);

		human.rotation_.y -= 0.05f;
		

		
		
		ball.rotation_.y -= 0.05f;
		
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
		light->Update();
		
		obj1->Update(&camera);
		obj2->Update(&camera);
		objskydome->Update(&camera);
		objground->Update(&camera);
		sprite->Update();
		sprite2->Update();
		camera.SetEye(eye);
		camera.Update();

		dxCommon->PreDraw();

		Object3D::PreDraw(dxCommon->GetCommandList());

		objskydome->Draw();
		objground->Draw();
		obj1->Draw();
		obj2->Draw();

		Object3D::PostDraw();

		spritecommon->PreDraw();
		
		sprite->Draw();
		sprite2->Draw();
		spritecommon->PostDraw();
		dxCommon->PostDraw();

	}
	
	delete input;
	delete obj1;
	delete obj2;
	delete model;
	delete model2;
	delete objskydome;
	delete objground;
	delete gra;
	delete skydome;
	delete sprite;
	delete sprite2;
	delete light;
	delete spritecommon;
	delete dxCommon;
	texturemanager->DeleteInstance();
	window->TerminateGameWindow();
}