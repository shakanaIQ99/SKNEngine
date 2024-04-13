#pragma once
#include "DxWindow.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include<vector>
#include"DescriptorHeap.h"
#include<memory>
#include"myMath.h"

using namespace Microsoft::WRL;
using namespace std;

namespace SKNEngine
{
	class DirectXCommon
	{

	private:

		DirectXCommon() = default;
		~DirectXCommon() = default;

		//コピーコンストラクタ・代入演算子削除
		DirectXCommon& operator=(const DirectXCommon&) = delete;
		DirectXCommon(const DirectXCommon&) = delete;
	public:



		static void Initialize(DxWindow* win);

		static void PreDraw(DxWindow* win);

		static void PostDraw();

		/// <returns>デバイス</returns>
		static ComPtr<ID3D12Device> GetDevice();

		/// <summary>
		/// 描画コマンドリストの取得
		/// </summary>
		/// <returns>描画コマンドリスト</returns>
		static ID3D12GraphicsCommandList* GetCommandList();

		static DescriptorHeap* GetDescriptorHeap();

		static size_t GetBackBufferCount();

		/// <summary>
		/// 背景の色をセット
		/// </summary>
		/// <param name="color">RGBA(初期値 { 0.1f , 0.25f , 0.5f , 0.0f } )</param>
		static void SetClearColor(Float4 color = { 0.1f,0.25f,0.5f,0.0f });

		//シングルトン
		static DirectXCommon* GetInstance();

	private:

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

		UINT64 fenceVal = 0;

		FLOAT clearColor[4] = { 0.1f,0.25f,0.5f,0.0f };	//色の指定はRGBAの0.0f～1.0f
	private:

		void InitializeDXGIdevice();

		void InitializeSwapChain(DxWindow* win);

		void InitializeCommand();

		void InitializeRenserTargetView();

		void InitializeDepthBuffer();

		void InitializeFence();

	};
}

