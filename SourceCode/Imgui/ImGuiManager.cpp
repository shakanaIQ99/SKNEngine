#include "ImGuiManager.h"
#include"imgui.h"
#include"imgui_impl_win32.h"
#include"imgui_impl_dx12.h"

void ImGuiManager::Initialize(HWND hwnd, DirectXCommon* dxcommon)
{
	assert(dxcommon);

	dxCommon = dxcommon;

	srvheap = dxCommon->GetDescriptorHeap()->GetHeap();

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	handle_ = dxCommon->GetDescriptorHeap()->AddSRV();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(dxCommon->GetDevice(), static_cast<int>(dxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvheap.Get(),
		handle_.cpuHandle,
		handle_.gpuHandle);
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
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	cmdList->SetDescriptorHeaps(1, srvheap.GetAddressOf());


	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);

}