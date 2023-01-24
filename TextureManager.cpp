#include "TextureManager.h"

void TextureManager::StaticInitialize(DirectXCommon* dxcommon)
{
	dxCommon = dxcommon;

	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

}
