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
	
	static TextureHandle Load(const std::string FilePath, const std::string Handle = "");

	static TextureData* GetTextureData(uint32_t handle);

	static TextureManager* GetInstance();

	void DeleteInstance();

private:

	TextureHandle LoadTexture(const string& FilePath, TextureHandle Handle);

	TextureHandle CreateHandle(TextureData TexData,TextureHandle Handle);

	
	static TextureManager* texManager;

	recursive_mutex mutex;
	map<TextureHandle, TextureData> textureMap;



	static vector<string>filePaths;

	static unordered_map<string, unique_ptr<TextureData>> texDatas;

	uint32_t textureSize;

};

