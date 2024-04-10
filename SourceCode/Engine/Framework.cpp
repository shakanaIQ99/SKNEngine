#include "Framework.h"

void Framework::Initialize()
{
	dxWindow = std::make_unique<DxWindow>();
	dxWindow->CreateGameWindow(title.c_str(), windowWidth, windowHeight);

	fps = std::make_unique<FPS>();
	fps->Initialize();

	SKNEngine::DirectXCommon::GetInstance()->Initialize(dxWindow.get());
	Input::Init(dxWindow.get());
	ImGuiManager::Initialize(dxWindow.get());
	OBJ3D::StaticInitialize();
	Draw3DLine::SetDevice(dxCommon->GetDevice(), dxCommon->GetCommandList());
	ParticleManager::StaticInitialize(dxCommon->GetDevice());
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	PostEffect::SetDXCommon(dxCommon);
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

void Framework::Destroy()
{
}

void Framework::Update()
{
}

bool& Framework::GetEndRequest()
{
	// TODO: return ステートメントをここに挿入します
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
}

void Framework::Run()
{
}
