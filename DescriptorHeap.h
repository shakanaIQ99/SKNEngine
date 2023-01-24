#pragma once
#include<wrl.h>
#include <d3d12.h>

using namespace Microsoft::WRL;

class DescriptorHeap
{
public:

	void Initialize(ID3D12Device* device);


	UINT64 CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource);

	ID3D12DescriptorHeap* GetHeap();

private:

	ComPtr <ID3D12Device> device;
	ComPtr <ID3D12DescriptorHeap> descHeap;
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	const size_t MaxSRV = 2056;

	size_t SRVCount = 0;

};

