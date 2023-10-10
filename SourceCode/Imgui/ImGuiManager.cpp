#include "ImGuiManager.h"

ComPtr<ID3D12DescriptorHeap> ImGuiManager::srvheap;
DescriptorHeap::DescriptorHeapViewHandle ImGuiManager::handle_;

void ImGuiManager::Initialize(HWND hwnd)
{

	srvheap = DirectXCommon::GetInstance()->GetDescriptorHeap()->GetHeap();

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	handle_ = DirectXCommon::GetInstance()->GetDescriptorHeap()->AddSRV();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(DirectXCommon::GetInstance()->GetDevice().Get(), static_cast<int>(DirectXCommon::GetInstance()->GetBackBufferCount()),
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

//void ImGuiManager::End()
//{
//	ImGui::Render();
//}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList().Get();
	ImGui::Render();

	cmdList->SetDescriptorHeaps(1, srvheap.GetAddressOf());


	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);

}