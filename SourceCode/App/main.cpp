#include"DxWindow.h"
#include"DirectXCommon.h"
//#include"Object3D.h"
#include "OBJ3D.h"
#include"Input.h"
#include<wrl.h>
#include"LightGroup.h"
#include<sstream>
#include<iomanip>
#include"ImGuiManager.h"
#include"FPS.h"
//#include"FbxLoader.h"
#include"ParticleManager.h"
#include"BulletManager.h"
#include"PostEffect.h"
#include"SpriteCommon.h"
#include"Draw3DLine.h"
#include "AudioManager.h"

#include "GameScene.h"

using namespace SKNEngine;


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
	ImGuiManager::Initialize(window->GetHwnd(), dxCommon);

	//FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	Input::Init(window->GetHInstance(), window->GetHwnd());
	OBJ3D::StaticInitialize(dxCommon->GetDevice());
	/*Object3D::SetDevice(dxCommon->GetDevice());
	Object3D::CreateGraphicsPipeline();*/
	Draw3DLine::SetDevice(dxCommon->GetDevice(),dxCommon->GetCommandList());
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	PostEffect::SetDXCommon(dxCommon);
	PostEffect::CreateGraphicsPipeline();
	AudioManager::StaticInitialize();

	

	PostEffect* postEffect = nullptr;
	postEffect = new PostEffect();
	postEffect->Initialize();

	unique_ptr<FPS>fps;
	fps = std::make_unique<FPS>();
	fps->Initialize();

	gameScene = new GameScene();
	gameScene->Init(dxCommon);

	
	while (true)
	{
		if (window->ProcessMessage())
		{
			break;
		}
		ImGuiManager::Begin();
		Input::InputUpdate();

		gameScene->Update();


		postEffect->PreDrawScene(dxCommon->GetCommandList());
		gameScene->Draw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCommandList());
		
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCommandList());
		//gameScene->Draw(dxCommon);

		ImGuiManager::Draw();
		dxCommon->PostDraw();

		fps->Update();

	}

	
	gameScene->Finalize();
	//FbxLoader::GetInstance()->Finalize();
	complete_type_safe_delete(gameScene);
	complete_type_safe_delete(postEffect);
	AudioManager::StaticFinalize();
	ImGuiManager::Finalize();
	complete_type_safe_delete(dxCommon);
	window->TerminateGameWindow();
}