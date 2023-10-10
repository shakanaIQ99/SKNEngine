#pragma once
#include "DxWindow.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include<vector>
#include"DescriptorHeap.h"
#include<memory>

using namespace Microsoft::WRL;
using namespace std;


class DirectXCommon
{
public:

	void Initialize(DxWindow* win, int32_t BackBufferWidth = DxWindow::window_width, int32_t BackBufferHeight = DxWindow::window_height);

	void PreDraw();

	void PostDraw();

	void Destroy();

	/// <returns>デバイス</returns>
	ComPtr<ID3D12Device> GetDevice() { return device; }

	/// <summary>
	/// 描画コマンドリストの取得
	/// </summary>
	/// <returns>描画コマンドリスト</returns>
	ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return commandList; }

	DescriptorHeap* GetDescriptorHeap() { return descHeap.get(); }

	size_t GetBackBufferCount()const { return backBuffers.size(); }

	//DescriptorHeap* GetDescriptorHeap()const { return descHeap.get(); }

	static DirectXCommon* GetInstance();

private:

	DxWindow* dxWin;

	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12Resource> depthBuff;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	

	vector<ComPtr<ID3D12Resource>> backBuffers;

	ComPtr<ID3D12Fence> fence;

	ComPtr<ID3D12Resource> depthBuffer;

	unique_ptr<DescriptorHeap> descHeap;

	UINT backBufferWidth = 0;
	UINT backBufferHeight = 0;

	UINT64 fenceVal = 0;

private:

	void InitializeDXGIdevice();

	void InitializeSwapChain();

	void InitializeCommand();

	void InitializeRenserTargetView();

	void InitializeDepthBuffer();

	void InitializeFence();

	DirectXCommon() = default;
	~DirectXCommon() = default;

	DirectXCommon& operator=(const DirectXCommon&) = delete;
	DirectXCommon(const DirectXCommon&) = delete;

};

