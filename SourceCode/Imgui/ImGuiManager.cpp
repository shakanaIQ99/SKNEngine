#include "ImGuiManager.h"
#include"DirectXCommon.h"

using namespace SKNEngine;

void ImGuiManager::Initialize(DxWindow* win)
{
	
	srvheap = DirectXCommon::GetInstance()->GetDescriptorHeap()->GetHeap();

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(win->GetHwnd());
	ImGui_ImplDX12_Init(DirectXCommon::GetInstance()->GetDevice().Get(), static_cast<int>(DirectXCommon::GetInstance()->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvheap.Get(),
		srvheap.Get()->GetCPUDescriptorHandleForHeapStart(),
		srvheap.Get()->GetGPUDescriptorHandleForHeapStart());
	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontDefault();
}

void ImGuiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	ImGui::Render();
}

void ImGuiManager::Draw()
{

	DirectXCommon::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, srvheap.GetAddressOf());


	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXCommon::GetInstance()->GetCommandList());

}

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}
