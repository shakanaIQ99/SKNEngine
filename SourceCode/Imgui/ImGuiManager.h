#pragma once
//ImGuiマネージャ
#include<Windows.h>
#include"DirectXCommon.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

using namespace Microsoft::WRL;
class ImGuiManager
{

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="hwnd"></param>
	static void Initialize(HWND hwnd);

	/// <summary>
	/// 解放
	/// </summary>
	static void Finalize();

	/// <summary>
	/// ImGuiの始め
	/// </summary>
	static void Begin();

	//static void End();

	/// <summary>
	/// 描画
	/// </summary>
	static void Draw();

private:

	static ComPtr<ID3D12DescriptorHeap>srvheap;

	static DescriptorHeap::DescriptorHeapViewHandle handle_;


	

};
