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
#include"TextureManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"

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

#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif	

	dxWindow = std::make_unique<DxWindow>();
	dxWindow->CreateGameWindow(title.c_str(), windowWidth, windowHeight);



	SKNEngine::DirectXCommon::Initialize(dxWindow.get());

	fps = std::make_unique<FPS>();
	fps->Initialize();

	Input::Init(dxWindow.get());
	ImGuiManager::GetInstance()->Initialize(dxWindow.get());
	OBJ3D::StaticInitialize();
	Draw3DLine::CreateGraphicsPipeline();
	ParticleManager::StaticInitialize();
	SpriteCommon::Initialize();
	PostEffect::CreateGraphicsPipeline();
	AudioManager::StaticInitialize();

	
#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(SKNEngine::DirectXCommon::GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}
#endif

	//シーンファクトリーを生成し、マネージャにセット
	AbstractSceneFactory* sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);

}

void Framework::Finalize()
{
	
	AudioManager::StaticFinalize();
	TextureManager::StaticFinalize();
	ImGuiManager::GetInstance()->Finalize();
	dxWindow->TerminateGameWindow();
}

void Framework::Update()
{
	if (!dxWindow->ProcessMessage())
	{
		endRequest = true;
	}

	Input::InputUpdate();

	SKNEngine::DirectXCommon::SetClearColor();
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
	windowWidth = static_cast<int32_t>(Width);//横幅
	windowHeight = static_cast<int32_t>(Height);//縦幅
}

void Framework::SetWindowColor(const Float4& color)
{
	SKNEngine::DirectXCommon::SetClearColor(color);
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

		SKNEngine::DirectXCommon::PreDraw(dxWindow.get());

		if (GetEndRequest())
		{
			break;
		}

		//描画処理
		PostEffectDraw();

#ifdef _DEBUG

		ImGuiManager::GetInstance()->Draw();

#endif _DEBUG

		SKNEngine::DirectXCommon::PostDraw();

		//FPS制御
		fps->Update();



	}
	Finalize();

}
