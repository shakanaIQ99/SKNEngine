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
#include"PostEffect.h"
#include"SpriteCommon.h"
#include"Draw3DLine.h"

#include "GameScene.h"
template <class T>
inline void complete_type_safe_delete(T * &p) {
	//  不完全な型のポインタをdeleteしようとした時にコンパイルエラーにする
	typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
	(void)sizeof(type_must_be_complete);

	delete p;
	p = nullptr;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	DxWindow* window = nullptr;
	unique_ptr<GameScene>gameScene;

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
	unique_ptr<DirectXCommon>dxCommon;
	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Initialize(window);
	ImGuiManager::Initialize(window->GetHwnd(), dxCommon.get());

	//FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	Input::Init(window->GetHInstance(), window->GetHwnd());

	OBJ3D::StaticInitialize(dxCommon->GetDevice());
	Object3D::SetDevice(dxCommon->GetDevice());
	Object3D::CreateGraphicsPipeline();
	Draw3DLine::SetDevice(dxCommon->GetDevice(),dxCommon->GetCommandList());
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	PostEffect::SetDXCommon(dxCommon.get());
	PostEffect::CreateGraphicsPipeline();

	
	unique_ptr<PostEffect>postEffect;
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();

	unique_ptr<FPS>fps;
	fps = std::make_unique<FPS>();
	fps->Initialize();

	gameScene = std::make_unique<GameScene>();
	gameScene->Init(dxCommon.get());

	
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
		gameScene->Draw(dxCommon.get());
		postEffect->PostDrawScene(dxCommon->GetCommandList());
		
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCommandList());
		//gameScene->Draw(dxCommon);

		ImGuiManager::Draw();
		dxCommon->PostDraw();

		fps->Update();

	}

	//FbxLoader::GetInstance()->Finalize();
	ImGuiManager::Finalize();
	window->TerminateGameWindow();
}