#pragma once
#include<Windows.h>
#include"DirectXCommon.h"

using namespace Microsoft::WRL;
class ImGuiManager
{

public:

	void Initialize(HWND hwnd, DirectXCommon* dxcommon);

	void Finalize();

	void Begin();

	void End();

	void Draw();

private:

	DirectXCommon* dxCommon;

	ComPtr<ID3D12DescriptorHeap>srvheap;

	DescriptorHeap::DescriptorHeapViewHandle handle_;


};
