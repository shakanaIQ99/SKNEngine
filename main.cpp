#include"DxWindow.h"
#include"DirectXCommon.h"
#include"Object3D.h"
#include"Input.h"
#include<wrl.h>
#include"Sprite2D.h"
#include"SpriteCommon.h"
#include"WorldTronsform.h"
#include"TextureManager.h"
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
	Model* model2 = Model::LoadFromOBJ("maru",true);
	Model* model = Model::LoadFromOBJ("chr_sword");
	WorldTransform wt1;
	WorldTransform wt2;
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

	Object3D* obj1 = nullptr;
	Object3D* obj2 = nullptr;
	obj1 = Object3D::Create(&wt1);
	obj1->SetModel(model);
	obj2 = Object3D::Create(&wt2);
	obj2->SetModel(model2);

	//
	sprite2->Wt->translation_.y = 5.0f;

	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}

		input->InputUpdate();

		if (input->GetKey(DIK_D))
		{
			sprite->Wt->translation_.x += 0.5f;
		}
		if (input->GetKey(DIK_A))
		{
			obj1->Wt->translation_.x -= 0.05f;
		}

		if (input->GetKey(DIK_Q))
		{
			obj1->Wt->rotation_.y += 0.1f;
		}
	
		obj1->Wt->rotation_.y -= 0.05f;
		
		if (input->GetKey(DIK_W))
		{
			obj1->Wt->rotation_.x += 0.1f;
		}
		if (input->GetKey(DIK_S))
		{
			obj1->Wt->rotation_.x -= 0.1f;
		}

		
		
		obj2->Wt->rotation_.y -= 0.05f;
		
		if (input->GetKey(DIK_RIGHT))
		{
			obj2->Wt->rotation_.y += 0.1f;
		}
		
		obj1->Update();
		obj2->Update();
		sprite->Update();
		sprite2->Update();

		dxCommon->PreDraw();

		Object3D::PreDraw(dxCommon->GetCommandList());

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
	delete sprite;
	delete sprite2;
	delete spritecommon;
	delete dxCommon;
	texturemanager->DeleteInstance();
	window->TerminateGameWindow();
}