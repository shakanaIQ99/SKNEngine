#include"DxWindow.h"
#include"DirectXCommon.h"
#include"Object3D.h"
#include"Input.h"
#include<wrl.h>
#include"LightGroup.h"
#include<sstream>
#include<iomanip>
#include"ImGuiManager.h"
#include"imgui.h"
#include"FPS.h"
#include"FbxLoader.h"
#include <ParticleManager.h>


#include "GameScene.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	DxWindow* window = nullptr;
	GameScene* gameScene = nullptr;

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

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	Object3D::SetDevice(dxCommon->GetDevice());
	Object3D::CreateGraphicsPipeline();
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	LightGroup::StaticInitialize(dxCommon->GetDevice());

	

	unique_ptr<FPS>fps;
	fps = std::make_unique<FPS>();
	fps->Initialize();

	gameScene = new GameScene();
	gameScene->Init(window, dxCommon);

	
	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}
		gameScene->Update();
		
		dxCommon->PreDraw();

		gameScene->Draw(dxCommon);
		dxCommon->PostDraw();

		fps->Update();

	}

	
	gameScene->Finalize();
	FbxLoader::GetInstance()->Finalize();
	delete gameScene;
	delete dxCommon;
	window->TerminateGameWindow();
}