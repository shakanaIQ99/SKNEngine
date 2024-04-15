#pragma once
#include "DirectXCommon.h"
#include <memory>
#include <DirectXTex.h>
#include <string>
#include <map>
#include <mutex>
#include "Float4.h"
#include <vector>

using namespace std;

using namespace Microsoft::WRL;

using namespace DirectX;

typedef string TextureHandle;

struct TextureData
{
	ComPtr<ID3D12Resource> texResource;

	int Index = -1;

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE();
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE();

	string filePath;
};

class TextureManager
{

public:

	static TextureManager* GetInstance(); 
	
	static TextureHandle Load(const std::string FilePath, const std::string Handle = "");
	static TextureData& GetTextureData(const TextureHandle& Handle);

	static void StaticFinalize();

private:

	TextureManager() {};
	~TextureManager() = default;
	TextureManager(const TextureManager&) {};
	TextureManager& operator=(const TextureManager&) { return *this; }


	TextureHandle LoadTexture(const string& FilePath, TextureHandle Handle);

	TextureHandle CreateHandle(TextureData TexData,TextureHandle Handle);

	TextureData& Get(const TextureHandle& handle);

	[[nodiscard]]
	ID3D12Resource* UploadTextureData(ID3D12Resource* Texture, const ScratchImage& MipImages);

	recursive_mutex mutex;
	map<TextureHandle, TextureData> textureMap;


};

