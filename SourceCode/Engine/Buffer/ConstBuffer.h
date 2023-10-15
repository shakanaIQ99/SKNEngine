#pragma once
//定数バッファ
#include<wrl.h>
#include <d3d12.h>
#include"DirectXCommon.h"
using namespace Microsoft::WRL;

class ConstBuffer
{

public:
	ConstBuffer() = default;
	~ConstBuffer() = default;

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="dxcommon"></param>
	/// <param name="size"></param>
	void Create(DirectXCommon* dxcommon, size_t size);

	/// <summary>
	/// GPUアドレス
	/// </summary>
	/// <returns></returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const;

	/// <summary>
	/// ViewDesc取得
	/// </summary>
	/// <returns></returns>
	D3D12_CONSTANT_BUFFER_VIEW_DESC GetViewDesc();

	//void Update(void* data);

	ID3D12Resource* GetResource();

	void* GetPtr();

private:

	
	ComPtr<ID3D12Resource> buffer;

	
	D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferView = {};

	void* bufferMappedPtr = nullptr;

	size_t bufferSize;

	ConstBuffer(const ConstBuffer&) = delete;
	void operator = (const ConstBuffer&) = delete;

};

