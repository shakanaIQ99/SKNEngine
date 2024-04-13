#pragma once
#include<Windows.h>
#include"DxWindow.h"
#include<wrl.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

using namespace Microsoft::WRL;
class ImGuiManager
{

public:

	void Initialize(DxWindow* win);

	void Finalize();

	void Begin();

	void End();

	void Draw();

	static ImGuiManager* GetInstance();

private:

	ComPtr<ID3D12DescriptorHeap>srvheap;

	ImGuiManager() = default;
	~ImGuiManager() = default;

	//コピーコンストラクタ・代入演算子削除
	ImGuiManager& operator=(const ImGuiManager&) = delete;
	ImGuiManager(const ImGuiManager&) = delete;

};
