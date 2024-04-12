#include "TextureManager.h"
TextureManager* TextureManager::texManager = nullptr;
vector<string>TextureManager::filePaths;
unordered_map<string, unique_ptr<TextureData>> TextureManager::texDatas;

using namespace SKNEngine;


TextureHandle TextureManager::LoadTexture(const string& filePath)
{
	std::unique_lock<std::recursive_mutex> lock(mutex);
	HRESULT result;

	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(textureMap.begin(), textureMap.end(), [&](const std::pair<TextureHandle, TextureData>& p) {
		return p.second.filePath == filePath;
		});
	if (itr != textureMap.end()) {
		return itr->first;
	}
	lock.unlock();


	/// <summary>
	/// ////////
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>

	auto itreter = find_if(texDatas.begin(), texDatas.end(), [&](pair<const string, unique_ptr<TextureData, default_delete<TextureData>>>& ptr)
		{
			return ptr.second->filePath == filePath;
		});

	if (itreter == texDatas.end())
	{
		unique_ptr<TextureData> data;

		data.reset(LoadFromTextureData(filePath));
		data->texHandle = textureSize;
		data->filePath = filePath;

		texDatas[filePath] = move(data);
		filePaths[textureSize] = filePath;
		uint32_t handl = textureSize;
		textureSize++;


		return handl;
	}
	else
	{

		uint32_t modelHandle = texDatas[filePath]->texHandle;

		return modelHandle;
	}
}


TextureHandle TextureManager::Load(const std::string filePath, const std::string handle)
{
	return TextureManager::GetInstance()->LoadTexture(filePath);
}

TextureData* TextureManager::GetTextureData(uint32_t handle)
{
	return texDatas[filePaths[handle]].get();
}

TextureManager* TextureManager::GetInstance()
{
	if (!texManager)
	{
		texManager = new TextureManager();
	}

	return texManager;
}

void TextureManager::DeleteInstance()
{
	delete texManager;
}

void TextureManager::FileLoad(const string& filePath, TexMetadata& metadata, ScratchImage& scratchImg)
{
	wchar_t Filepath[256];
	HRESULT result;

	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, Filepath, _countof(Filepath));
	
	result = LoadFromWICFile(
		Filepath,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);
	assert(SUCCEEDED(result));


}

TextureData* TextureManager::LoadFromTextureData(const string& filePath)
{
	TextureData* texdata = new TextureData();
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};

	texdata->srvHeap = DirectXCommon::GetInstance()->GetDescriptorHeap()->GetHeap();

	FileLoad(filePath, metadata, scratchImg);

	
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format =MakeSRGB(metadata.format);

	texdata->texResource = CreateTexBuff(metadata, scratchImg);

	texdata->gpuHandle = CreateSRV(texdata->texResource.Get(), metadata);

	texdata->width = metadata.width;
	texdata->height = metadata.height;

	return texdata;
}

ComPtr<ID3D12Resource> TextureManager::CreateTexBuff(TexMetadata& metadata, ScratchImage& scratchImg)
{
	ComPtr<ID3D12Resource> texbuff;
	HRESULT result;

	D3D12_RESOURCE_DESC rsDesc{};
	rsDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rsDesc.Format = metadata.format;
	rsDesc.Width = metadata.width;
	rsDesc.Height = (UINT)metadata.height;
	rsDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	rsDesc.MipLevels = (UINT16)metadata.mipLevels;
	rsDesc.SampleDesc.Count = 1;

	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&texHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&rsDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(texbuff.ReleaseAndGetAddressOf())
	);

	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出
		result = texbuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	return texbuff;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::CreateSRV(ID3D12Resource* texResource, TexMetadata& metadata)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);

	D3D12_GPU_DESCRIPTOR_HANDLE srvgpudesc;

	srvgpudesc.ptr = DirectXCommon::GetInstance()->GetDescriptorHeap()->CreateSRV(srvDesc, texResource);

	return srvgpudesc;
}


