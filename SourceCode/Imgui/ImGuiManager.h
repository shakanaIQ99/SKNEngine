#pragma once
#include<Windows.h>
#include"DirectXCommon.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

using namespace Microsoft::WRL;
class ImGuiManager
{

public:

	static void Initialize(HWND hwnd, DirectXCommon* dxcommon);

	static void Finalize();

	static void Begin();

	//static void End();

	static void Draw();

private:

	static DirectXCommon* dxCommon;

	static ComPtr<ID3D12DescriptorHeap>srvheap;

	static DescriptorHeap::DescriptorHeapViewHandle handle_;


};
