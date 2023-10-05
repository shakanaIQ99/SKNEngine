#pragma once
#include<wrl.h>
#include <d3d12.h>

using namespace Microsoft::WRL;

class DescriptorHeap
{
public:

	struct DescriptorHeapViewHandle
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	};

	void Initialize(ID3D12Device* Device);


	UINT64 CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource);

	UINT64 CreateCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc);

	DescriptorHeapViewHandle AddSRV();

	ComPtr<ID3D12DescriptorHeap> GetHeap();

private:

	ComPtr <ID3D12Device> device;
	ComPtr <ID3D12DescriptorHeap> descHeap;
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	const size_t MaxSRV = 400000;
	const size_t MaxCBV = 600000;

	size_t SRVCount = 0;
	size_t CBVCount = 0;



	UINT incrementSize = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE	startGpuHandle;

};