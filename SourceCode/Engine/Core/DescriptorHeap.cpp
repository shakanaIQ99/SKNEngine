#include "DescriptorHeap.h"
#include<cassert>

void DescriptorHeap::Initialize(ID3D12Device* Device)
{
	device = Device;
	HRESULT result = S_FALSE;

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = static_cast<UINT>(MaxSRV + MaxCBV);
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(descHeap.ReleaseAndGetAddressOf()));//生成
	assert(SUCCEEDED(result));

	startCpuHandle = descHeap->GetCPUDescriptorHandleForHeapStart();
	startGpuHandle = descHeap->GetGPUDescriptorHandleForHeapStart();

	incrementSize = static_cast<UINT>(device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

}

UINT64 DescriptorHeap::CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource)
{
	if (SRVCount > MaxSRV)
	{
		assert(0);
	}
	//SRVヒープの先頭ハンドルを取得
	DescriptorHeapViewHandle handle{};

	//ハンドルのポインタずらし
	handle.cpuHandle.ptr = startCpuHandle.ptr + (static_cast<SIZE_T>(SRVCount) * incrementSize);
	handle.gpuHandle.ptr = startGpuHandle.ptr + (static_cast<SIZE_T>(SRVCount) * incrementSize);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(resource, &desc, handle.cpuHandle);
	SRVCount++;

	return handle.gpuHandle.ptr;
}

UINT64 DescriptorHeap::CreateCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc)
{
	if (CBVCount > MaxCBV)
	{
		CBVCount = 0;
	}

	//SRVヒープの先頭ハンドルを取得
	DescriptorHeapViewHandle handle{};

	//ハンドルのポインタずらし
	handle.cpuHandle.ptr = startCpuHandle.ptr + (static_cast<SIZE_T>(MaxSRV + CBVCount) * incrementSize);
	handle.gpuHandle.ptr = startGpuHandle.ptr + (static_cast<SIZE_T>(MaxSRV + CBVCount) * incrementSize);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateConstantBufferView(&desc, handle.cpuHandle);

	CBVCount++;

	return handle.gpuHandle.ptr;
}

DescriptorHeap::DescriptorHeapViewHandle DescriptorHeap::AddSRV()
{
	if (SRVCount > MaxSRV)
	{
		assert(0);
	}
	//SRVヒープの先頭ハンドルを取得
	DescriptorHeapViewHandle handle{};

	//ハンドルのポインタずらし
	handle.cpuHandle.ptr = startCpuHandle.ptr + (static_cast<SIZE_T>(SRVCount) * incrementSize);
	handle.gpuHandle.ptr = startGpuHandle.ptr + (static_cast<SIZE_T>(SRVCount) * incrementSize);

	SRVCount++;

	return handle;
}


ComPtr<ID3D12DescriptorHeap> DescriptorHeap::GetHeap()
{
	return descHeap;
}