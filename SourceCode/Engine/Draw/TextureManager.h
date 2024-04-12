#pragma once
#include "DirectXCommon.h"
#include <memory>
#include <DirectXTex.h>
#include <string>
#include <map>
#include <mutex>
#include "Float4.h"

using namespace std;

using namespace Microsoft::WRL;

using namespace DirectX;

typedef string TextureHandle;

struct TextureData
{
	ComPtr<ID3D12Resource> texResource;

	ComPtr <ID3D12DescriptorHeap> srvHeap;

	UINT heapIndex = -1;

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE();
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE();

	string filePath;
};

class TextureManager
{

public:

public:
	//TextureManagerを取得する
	static TextureManager* GetInstance() {
		static TextureManager instance;
		return &instance;
	}
	
	static TextureHandle Load(const std::string filePath, const std::string handle = "");

	static TextureData* GetTextureData(uint32_t handle);

	static TextureManager* GetInstance();

	void DeleteInstance();

private:

	TextureHandle LoadTexture(const string& filePath);

	void FileLoad(const string& filePath, TexMetadata& metadata, ScratchImage& scratchImg);

	TextureData* LoadFromTextureData(const string& filePath);

	ComPtr<ID3D12Resource>CreateTexBuff(TexMetadata& metadata, ScratchImage& scratchImg);

	D3D12_GPU_DESCRIPTOR_HANDLE CreateSRV(ID3D12Resource* texResource, TexMetadata& metadata);

	
	static TextureManager* texManager;

	recursive_mutex mutex;
	map<TextureHandle, TextureData> textureMap;

	D3D12_HEAP_PROPERTIES texHeapProp{};

	static vector<string>filePaths;

	static unordered_map<string, unique_ptr<TextureData>> texDatas;

	uint32_t textureSize;

};

