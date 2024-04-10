#pragma once
#include<Windows.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

using namespace Microsoft::WRL;
class ImGuiManager
{

public:

	static void Initialize(DxWindow* win);

	static void Finalize();

	static void Begin();

	//static void End();

	static void Draw();

private:

	static ComPtr<ID3D12DescriptorHeap>srvheap;

};
