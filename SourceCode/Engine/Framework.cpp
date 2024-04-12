#include "Framework.h"
#include"OBJ3D.h"
#include"Input.h"
#include"ParticleManager.h"
#include"BulletManager.h"
#include"PostEffect.h"
#include"SpriteCommon.h"
#include"ImGuiManager.h"
#include"FPS.h"
#include"Draw3DLine.h"
#include"AudioManager.h"
#include"LightGroup.h"

template <class T>
inline void complete_type_safe_delete(T*& p) {
	//  不完全な型のポインタをdeleteしようとした時にコンパイルエラーにする
	typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
	(void)sizeof(type_must_be_complete);

	delete p;
	p = nullptr;
}
void Framework::Initialize()
{
	dxWindow = std::make_unique<DxWindow>();
	dxWindow->CreateGameWindow(title.c_str(), windowWidth, windowHeight);

	fps = std::make_unique<FPS>();
	fps->Initialize();

	SKNEngine::DirectXCommon::GetInstance()->Initialize(dxWindow.get());
	Input::Init(dxWindow.get());
	ImGuiManager::GetInstance()->Initialize(dxWindow.get());
	OBJ3D::StaticInitialize();
	Draw3DLine::CreateGraphicsPipeline();
	ParticleManager::StaticInitialize();
	SpriteCommon::Initialize();
	PostEffect::CreateGraphicsPipeline();
	AudioManager::StaticInitialize();





#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif	

}

void Framework::Finalize()
{
	
	AudioManager::StaticFinalize();
	ImGuiManager::GetInstance()->Finalize();
	SKNEngine::DirectXCommon::GetInstance()->Finalize();
	dxWindow->TerminateGameWindow();
}

void Framework::Update()
{
	if (!dxWindow->ProcessMessage())
	{
		endRequest = true;
	}

	Input::InputUpdate();

	SKNEngine::DirectXCommon::GetInstance()->SetClearColor();
}
bool& Framework::GetEndRequest()
{
	return endRequest;
}

void Framework::SetWindowData(const std::string& Title, const float Width, const float Height)
{
	wchar_t wtitle[256];

	MultiByteToWideChar(CP_ACP, 0, Title.c_str(), -1, wtitle, _countof(wtitle));

	title = wtitle;
	windowWidth = Width;//横幅
	windowHeight = Height;//縦幅
}

void Framework::SetWindowColor(const Float4& color)
{
	SKNEngine::DirectXCommon::GetInstance()->SetClearColor(color);
}

void Framework::Run()
{
	Initialize();

	while (true)
	{
#ifdef _DEBUG
		//更新処理
		ImGuiManager::GetInstance()->Begin();
#endif _DEBUG

		Update();

		SceneDraw();

#ifdef _DEBUG
		ImGuiManager::GetInstance()->End();
#endif _DEBUG

		SKNEngine::DirectXCommon::GetInstance()->PreDraw(dxWindow.get());

		if (GetEndRequest())
		{
			break;
		}

		//描画処理
		PostEffectDraw();

#ifdef _DEBUG

		ImGuiManager::GetInstance()->Draw();

#endif _DEBUG

		SKNEngine::DirectXCommon::GetInstance()->PostDraw();

		//FPS制御
		fps->Update();



	}
	Finalize();

}
