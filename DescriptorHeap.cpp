#include "DescriptorHeap.h"
#include<cassert>

void DescriptorHeap::Initialize(ID3D12Device* Device)
{
	device = Device;
	HRESULT result = S_FALSE;

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = static_cast<UINT>(MaxSRV);
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	assert(SUCCEEDED(result));

}

UINT64 DescriptorHeap::CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource)
{
	if (SRVCount > MaxSRV)
	{
		assert(0);
	}
	D3D12_CPU_DESCRIPTOR_HANDLE	cpuHandle = descHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE	gpuHandle = descHeap->GetGPUDescriptorHandleForHeapStart();

	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	cpuHandle.ptr += static_cast<UINT64>(SRVCount) * incrementSize;
	gpuHandle.ptr += static_cast<UINT64>(SRVCount) * incrementSize;

	device->CreateShaderResourceView(resource, &desc, cpuHandle);
	SRVCount++;

	return gpuHandle.ptr;
}

ID3D12DescriptorHeap* DescriptorHeap::GetHeap()
{
	return descHeap.Get();
}
