#pragma once
//テクスチャマネージャ
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

	/// <summary>
	/// 静的初期化
	/// </summary>
	void StaticInitialize();
	
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	uint32_t LoadTexture(const string& path);

	/// <summary>
	/// 静的テクスチャ読み込み
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	static uint32_t Load(const string& path);

	/// <summary>
	/// テクスチャデータ取得
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	static TextureData* GetTextureData(uint32_t handle);

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();


private:

	/// <summary>
	/// ファイル読み込み
	/// </summary>
	/// <param name="path"></param>
	/// <param name="metadata"></param>
	/// <param name="scratchImg"></param>
	void FileLoad(const string& path, TexMetadata& metadata, ScratchImage& scratchImg);

	/// <summary>
	/// 読み込んだテクスチャデータを取得
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	TextureData* LoadFromTextureData(const string& path);

	/// <summary>
	/// テクスチャバッファを生成
	/// </summary>
	/// <param name="metadata"></param>
	/// <param name="scratchImg"></param>
	/// <returns></returns>
	ComPtr<ID3D12Resource>CreateTexBuff(TexMetadata& metadata, ScratchImage& scratchImg);

	/// <summary>
	/// SRVを生成
	/// </summary>
	/// <param name="texBuff"></param>
	/// <param name="metadata"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE CreateSRV(ID3D12Resource* texBuff, TexMetadata& metadata);

	static std::unique_ptr<TextureManager> texManager;

	D3D12_HEAP_PROPERTIES texHeapProp{};

	static vector<string>FilePaths;

	static unordered_map<string, unique_ptr<TextureData>> texDatas;

	uint32_t TextureSize;

};

