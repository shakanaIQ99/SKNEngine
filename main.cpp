#include"DxWindow.h"
#include"DirectXCommon.h"
#include"Object3D.h"
#include"Input.h"
#include<wrl.h>
//#include"Sprite2D.h"
//#include"SpriteCommon.h"
#include"WorldTronsform.h"
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

	//SpriteCommon* spritecommon = nullptr;
	//spritecommon = new SpriteCommon();
	//spritecommon->Initialize(dxCommon);
	//spritecommon->LoadTexture(0, L"Resources/visual.png");
	//
	Object3D::StaticInitialize(dxCommon->GetDevice(), DxWindow::window_width, DxWindow::window_height);
	Model* model2 = Model::LoadFromOBJ("maru",true);
	Model* model = Model::LoadFromOBJ("chr_sword");
	//Object3D::LoadTexture(1, L"Resources/visual.png");
	WorldTransform wt1;
	WorldTransform wt2;
	//Sprite2D* sprite = nullptr;
	//sprite = new Sprite2D();
	//sprite->Initialize(spritecommon,&wt1);
	//Sprite2D* sprite2 = nullptr;
	//sprite2 = new Sprite2D();
	//sprite2->Initialize(spritecommon, &wt2);

	//WorldTransform wt2;

	Object3D* obj1 = nullptr;
	Object3D* obj2 = nullptr;
	obj1 = Object3D::Create(0,&wt1);
	obj1->SetModel(model);
	obj2 = Object3D::Create(0,&wt2);
	obj2->SetModel(model2);

	//
	//sprite2->Wt->translation_.y = 5.0f;

	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}

		input->InputUpdate();

		//if (input->GetKey(DIK_D))
		//{
		//	sprite->Wt->translation_.x += 0.05f;
		//}
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
		//sprite->Update();
		//sprite2->Update();

		dxCommon->PreDraw();
		//spritecommon->PreDraw();
		Object3D::PreDraw(dxCommon->GetCommandList());

		obj1->Draw();
		obj2->Draw();

		Object3D::PostDraw();
		//sprite->Draw();
		//sprite2->Draw();
		//spritecommon->PostDraw();
		dxCommon->PostDraw();

	}
	
	delete input;
	delete obj1;
	delete obj2;
	delete model;
	delete model2;
	//delete sprite;
	//delete spritecommon;
	delete dxCommon;

	window->TerminateGameWindow();
}