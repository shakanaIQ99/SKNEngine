#pragma once
#include"DirectXCommon.h"
#include<memory>
#include<DirectXTex.h>
#include <DirectXMath.h>
#include<string>
#include <unordered_map>

using namespace std;

using namespace Microsoft::WRL;

using namespace DirectX;

struct TextureData
{
	ComPtr<ID3D12Resource> texBuff;

	ComPtr <ID3D12DescriptorHeap> srvHeap;

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

	size_t width = 0;

	size_t height = 0;

	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };

	string path;

	uint32_t texHandle;

	
};

class TextureManager
{

public:

	void StaticInitialize(DirectXCommon* dxcommon);
	
	uint32_t LoadTexture(const string& path);

	static uint32_t Load(const string& path);

	static TextureData* GetTextureData(uint32_t handle);

	static TextureManager* GetInstance();

	void DeleteInstance();

private:

	void FileLoad(const string& path, TexMetadata& metadata, ScratchImage& scratchImg);

	TextureData* LoadFromTextureData(const string& path);

	ComPtr<ID3D12Resource>CreateTexBuff(TexMetadata& metadata, ScratchImage& scratchImg);

	D3D12_GPU_DESCRIPTOR_HANDLE CreateSRV(ID3D12Resource* texBuff, TexMetadata& metadata);

	DirectXCommon* dxCommon;

	static TextureManager* texManager;

	D3D12_HEAP_PROPERTIES texHeapProp{};

	static vector<string>FilePaths;

	static unordered_map<string, unique_ptr<TextureData>> texDatas;

	uint32_t TextureSize;

	string fileExt_;

};

